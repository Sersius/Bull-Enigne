#include "Application.h"
#include "ModuleLoadFBX.h"
#include "ModuleSceneIntro.h"

#include "ModuleGameObject.h"
#include "Material.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

#include "Glew/include/glew.h" 
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"
#include "ParShapes/par_shapes.h"
#include "MathGeoLib/include/Geometry/AABB.h"

#pragma comment (lib, "Assimp\\libx86\\assimp.lib")
#pragma comment (lib, "Devil\\libx86\\DevIL.lib")
#pragma comment ( lib, "Devil\\libx86\\ILU.lib" )
#pragma comment ( lib, "Devil\\libx86\\ILUT.lib" )

ModuleLoadFBX::ModuleLoadFBX(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleLoadFBX::~ModuleLoadFBX()
{}

// Load assets
bool ModuleLoadFBX::Start()
{
	bool ret = true;
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return ret;
}
update_status ModuleLoadFBX::PreUpdate(float dt)
{

	return(UPDATE_CONTINUE);
}

// Load assets
bool ModuleLoadFBX::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

// Update
update_status ModuleLoadFBX::Update(float dt)
{
	
	return UPDATE_CONTINUE;
}

bool ModuleLoadFBX::LoadFbx(const char* path,GameObject*go)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes()) {
	
		aiNode* rootNode = scene->mRootNode;
		for (int i = 0; i < rootNode->mNumChildren; ++i)
		{
			
				LoadModelInfo(scene,rootNode->mChildren[i], go, path);
			
		}
		return true;
	}
	else
	{
		return false;
	}
	
}

void ModuleLoadFBX::LoadModelInfo(const aiScene* scene, aiNode* node,GameObject* game_object,const char* path)
{
	this->path = path;
	std::string name(path);
	std::string name_fbx = name.substr(name.find_first_of("/") + 1);
	
	if (node->mNumMeshes> 0) {   	
		
			aiMesh* new_mesh = scene->mMeshes[node->mMeshes[0]];
			
			//CHECK FOR MATERIAL
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			if (material != nullptr) {
				uint numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
				aiString path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				texture_path = path.data;
				std::string output_file;
				ImportTexture(texture_path.c_str(), "Assets/Textures/", output_file);
				game_object->CreateComponent(COMPONENT_TYPE::MATERIAL);
				game_object->material->GetTexture(texture_path.c_str());
			
			}
			name_mesh =  node->mName.C_Str();
			mesh = InfoFbx();

			mesh.num_vertex = new_mesh->mNumVertices;
			mesh.vertex = new float[mesh.num_vertex * 3];
			memcpy(mesh.vertex, new_mesh->mVertices, sizeof(float)*mesh.num_vertex * 3);
			
			if (new_mesh->HasFaces()) {
				mesh.num_index = new_mesh->mNumFaces * 3;
				mesh.index = new uint[mesh.num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else    
						memcpy(&mesh.index[i*3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				} 
			}
			if (new_mesh->HasTextureCoords(mesh.id_uvs))
			{
				// ---- UVs ----
				mesh.num_uvs = new_mesh->mNumVertices;
				mesh.uvs = new float[mesh.num_uvs * 2];

				for (int i = 0; i < new_mesh->mNumVertices; ++i)
				{
					memcpy(&mesh.uvs[i * 2], &new_mesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&mesh.uvs[(i * 2) + 1], &new_mesh->mTextureCoords[0][i].y, sizeof(float));
				}
			}
			
			if (new_mesh->HasNormals()) {
				mesh.num_normals = new_mesh->mNumVertices;
				mesh.normals = new float[mesh.num_normals * 3]; // assume each face is a triangle
				memcpy(mesh.normals, new_mesh->mNormals, sizeof(float)*mesh.num_normals * 3);
			}
			
		
		CreateBuffers();
	
	
		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotation;
		node->mTransformation.Decompose(scaling, rotation, translation);
		aiMatrix3x3 rotMat = rotation.GetMatrix();
		aiVector3D rotationEuler = rotMat.GetEuler();

		float3 pos(translation.x, translation.y, translation.z);
		float3 scale(scaling.x, scaling.y, scaling.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);

		if (scene->mNumMeshes != 0)
		{
			childGO = App->scene_intro->CreateGameObject(game_object);
			childGO->CreateComponent(COMPONENT_TYPE::MESH);
			childGO->mesh->info_mesh = mesh;
			childGO->mesh->path = path;
			childGO->material = game_object->material;
			childGO->SetName(name_mesh.c_str());
			childGO->transform->position = pos;
			childGO->transform->rotation = rot;
			childGO->transform->scale = scale;
			childGO->is_primitive = false;
			
		}

		ImporterMesh(name, childGO->mesh, name_mesh);
		App->renderer3D->meshes.push_back(childGO->mesh);
		LOG("Mesh name: %s", name_mesh.c_str());
		LOG("Loaded mesh with %i vertices.", mesh.num_vertex);
		LOG("Loaded mesh with %i indices.", mesh.num_index);
		LOG("Loaded mesh with %i triangles.", mesh.num_vertex / 3);
		LOG("Loaded mesh with %i normals.", mesh.num_normals);
		LOG("Loaded mesh with %i uvs.", mesh.num_uvs);
		
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		LoadModelInfo(scene, node->mChildren[i], game_object,path);
	}

}

InfoFbx ModuleLoadFBX::LoadParShapeMesh(par_shapes_mesh* mesh)
{
	InfoFbx par_mesh = InfoFbx();

	//Vertex
	par_mesh.num_vertex = mesh->npoints;
	par_mesh.vertex = new float[par_mesh.num_vertex * 3];
	memcpy(par_mesh.vertex, mesh->points, sizeof(float) * mesh->npoints * 3);

	//Faces
	par_mesh.num_normals = mesh->ntriangles;
	par_mesh.num_index = mesh->ntriangles * 3;
	par_mesh.index = new uint[par_mesh.num_index];

	for (int i = 0; i < par_mesh.num_index; i++)
	{
		par_mesh.index[i] = (uint)mesh->triangles[i];
	}

	glGenBuffers(1, (GLuint*) &(par_mesh.id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, par_mesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * par_mesh.num_vertex, par_mesh.vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(par_mesh.id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, par_mesh.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * par_mesh.num_index, par_mesh.index, GL_STATIC_DRAW);

	childGO = App->scene_intro->CreateGameObject(App->scene_intro->root);
	childGO->CreateComponent(COMPONENT_TYPE::MESH);
	childGO->CreateComponent(COMPONENT_TYPE::MATERIAL);
	childGO->mesh->info_mesh = par_mesh;
	childGO->SetName("Primitive");
	childGO->is_primitive = true;
	App->renderer3D->meshes.push_back(childGO->mesh);
	return par_mesh;
}

bool ModuleLoadFBX::LoadTexture(std::string path_texture, uint& texture_id)
{
	uint id = 0;

	ilGenImages(1, &id);
	ilBindImage(id);

	if(ilLoad(IL_TYPE_UNKNOWN, path_texture.c_str()))
	{
		LOG("Texture loaded correctly");
	}
	else
	{
		LOG("Couldn't load texture");
		return false;
	}

	texture_width = ilGetInteger(IL_IMAGE_WIDTH);
	texture_height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture_id = ilutGLBindTexImage();
	glBindTexture(GL_TEXTURE_2D, 0);
	ilDeleteImages(1, &id);
	return true;
}

void ModuleLoadFBX::FocusGameObject()
{
	if (App->scene_intro->selected != nullptr)
	{
		if (App->scene_intro->selected->mesh != nullptr && App->scene_intro->selected->transform != nullptr)
		{
			AABB aux_box = App->scene_intro->selected->GetAABB();

			float3 reference = aux_box.CenterPoint();

			float3 position = aux_box.CenterPoint() + aux_box.maxPoint;

			App->camera->dummy->frustum.pos = position;
			App->camera->LookAt(reference);
		}
	}
}

bool ModuleLoadFBX::ImportTexture(const char * file, const char * path, std::string & output_file)
{
	bool ret = false;
	std::string name(file);
	std::string path2(path);
	std::string name_texture = name.substr(name.find_last_of("\\") + 1);
	std::string::size_type const p(name_texture.find_last_of('.'));
	std::string file_without_extension = name_texture.substr(0, p);

	char* buffer = nullptr;
	uint lenght = App->fileSystem->Load((path2 + name_texture).c_str(), &buffer);

	if (ilLoadL(IL_TYPE_UNKNOWN, (const void*)buffer, lenght))
	{
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
		if (size > 0) {
			data = new ILubyte[size]; // allocate data buffer

			if (ilSaveL(IL_DDS, data, size) > 0) {
				ret = App->fileSystem->SaveUnique(output_file, data, size, LIBRARY_TEXTURES_FOLDER, file_without_extension.c_str(), ".dds");
				delete data;
			}
		}
	}
	return ret;
}
void ModuleLoadFBX::ImporterMesh(std::string & output_file, Mesh* mesh, std::string name)
{
	//VERTEX -> INDEX -> UV'S -> NORMALS	
	uint ranges[4] = { mesh->info_mesh.num_vertex, mesh->info_mesh.num_index,  mesh->info_mesh.num_uvs, mesh->info_mesh.num_normals };
	uint size = sizeof(ranges) + sizeof(float) * mesh->info_mesh.num_vertex * 3 + sizeof(uint) * mesh->info_mesh.num_index + sizeof(float) *mesh->info_mesh.num_uvs * 2 + sizeof(float) *mesh->info_mesh.num_normals * 3;

	char* data = new char[size]; // Allocate 
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges 
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices

	//Store Vertex
	bytes = sizeof(float) *  mesh->info_mesh.num_vertex * 3;
	memcpy(cursor, mesh->info_mesh.vertex, bytes);
	cursor += bytes;

	//Store Index
	bytes = sizeof(uint) *  mesh->info_mesh.num_index;
	memcpy(cursor, mesh->info_mesh.index, bytes);
	cursor += bytes;

	//Store UV'S
	bytes = sizeof(float) *  mesh->info_mesh.num_uvs * 2;
	memcpy(cursor, mesh->info_mesh.uvs, bytes);
	cursor += bytes;

	//Store Normals
	bytes = sizeof(float) *  mesh->info_mesh.num_normals * 3;
	memcpy(cursor, mesh->info_mesh.normals, bytes);
	
	App->fileSystem->SaveUnique(output_file, data, size, LIBRARY_MESH_FOLDER, name.c_str(), ".bl");
	mesh->final_path = output_file.c_str();
}
bool ModuleLoadFBX::LoadMesh(const void * buffer, GameObject* go)
{
	char* cursor = (char*)buffer;
	//VERTEX -> INDEX -> UV'S -> NORMALS	
	uint ranges[4];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	go->mesh->info_mesh.num_vertex = ranges[0];
	go->mesh->info_mesh.num_index = ranges[1];
	go->mesh->info_mesh.num_uvs = ranges[2];
	go->mesh->info_mesh.num_normals = ranges[3];

	//Load Vertices
	bytes = sizeof(float) * go->mesh->info_mesh.num_vertex * 3;
	go->mesh->info_mesh.vertex = new float[go->mesh->info_mesh.num_vertex * 3];
	memcpy(go->mesh->info_mesh.vertex, cursor, bytes);

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * go->mesh->info_mesh.num_index;
	go->mesh->info_mesh.index = new uint[go->mesh->info_mesh.num_index];
	memcpy(go->mesh->info_mesh.index, cursor, bytes);

	//Load UV's
	cursor += bytes;
	bytes = sizeof(float) * go->mesh->info_mesh.num_uvs * 2;
	go->mesh->info_mesh.uvs = new float[go->mesh->info_mesh.num_uvs * 2];
	memcpy(go->mesh->info_mesh.uvs, cursor, bytes);
	//Load Normals
	cursor += bytes;
	bytes = sizeof(float) * go->mesh->info_mesh.num_normals * 3;
	go->mesh->info_mesh.normals = new float[go->mesh->info_mesh.num_normals * 3];
	memcpy(go->mesh->info_mesh.normals, cursor, bytes);

	//CreateBuffers();
	return true;
}
void ModuleLoadFBX::CreateBuffers()
{
	glGenBuffers(1, (GLuint*) &(mesh.id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertex, mesh.vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) &(mesh.id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh.num_index, mesh.index, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(mesh.id_uvs));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh.num_uvs, mesh.uvs, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(mesh.id_normals));
	glBindBuffer(GL_ARRAY_BUFFER, mesh.id_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_normals, mesh.normals, GL_STATIC_DRAW);
}
bool ModuleLoadFBX::ImportMesh(const char* path,GameObject* go)
{
	char* buffer;
	uint size = App->fileSystem->Load(path, &buffer);
	if (size > 0)
	{
		 LoadMesh(buffer,go);	
	}
	return true;
}