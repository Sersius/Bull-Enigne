#pragma once


#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "ModuleLoadFBX.h"
#include "ModuleFileSystem.h"
#include "ModuleAudio.h"
#include "SerializationScene.h"

#include <list>

struct SerializationScene;

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* UI;
	ModuleLoadFBX* loadFBX;
	ModuleFileSystem* fileSystem;
	ModuleAudio* audio;
	SerializationScene* serialization;

	char* app_name;
	const char* title;
	int width = 0;
	int height = 0;
	char* organization;

private:

	Timer	ms_timer;
	float	dt = 0.0f;
	float last_FPS = 0.0f;
	float last_ms = 0.0f;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetMS();
	float GetFPS();
	float GetDT() const;
	uint GenRandomNumber();
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
extern Application* App;