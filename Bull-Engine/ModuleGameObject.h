#ifndef __MODULEGAMEOBJECT_H__
#define __MODULEGAMEOBJECT_H__

#include "Globals.h"
#include "Config.h"
#include "MathGeoLib/include/MathGeoLib.h"


#include <vector>
#include <string>


enum COMPONENT_TYPE
{
	NONE = 0,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	AUDIO_EMITTER,
	AUDIO_LISTENER,
	REVERB_ZONE
};

class Component;
class Material;
class Mesh;
class Transform;
class Camera;
class AudioEmitter;
class AudioListener;
class ReverbZone;

class GameObject
{
public:

	GameObject(GameObject* parent = nullptr);
	~GameObject();

	void Update(float dt);

	Component* CreateComponent(COMPONENT_TYPE type, char* name="");
	Material* GetComponentMaterial() const;
	Mesh* GetComponentMesh() const;
	Transform* GetComponentTransform() const;
	Camera* GetComponentCamera() const;
	AudioEmitter* GetComponentAudioEmitter() const;
	AudioListener* GetComponentAudioListener() const;
	ReverbZone* GetComponentReverbZone() const;
	Component* GetComponent(COMPONENT_TYPE type) const;
	void SetName(const char* new_name);
	void SetActive(bool active);
	const char* GetName()const;
	void BlitHierarchy(GameObject* root);
	void BoundingBox();
	math::AABB GetAABB();
	void DrawBoundingBox();
	void SaveInfoGameObject(GameObject* go, JSON_Array* json_array);
	void DeleteScene();
	void LoadInfoGambeObject(JSON_Object* obj, GameObject* go);
	void MoveGO();
public:
	math::AABB bounding_box;
	math::OBB obb;
	std::vector<GameObject*> children;
	Material* material = nullptr;
	Mesh* mesh = nullptr;
	Transform* transform = nullptr;
	Camera* camera = nullptr;
	AudioEmitter* audio_emitter = nullptr;
	AudioListener* audio_listener = nullptr;
	ReverbZone* reverb_zone = nullptr;
	bool render_model = true;
	bool is_primitive = false;
	uint uuid = 0;
	uint uuid_parent = 0;
	bool go_left = false;
	bool go_right = true;
	std::string name;
	GameObject* parent = nullptr;
	
	std::vector<Component*> components;
	bool active = true;

	bool is_selected = false;
private:


};

#endif