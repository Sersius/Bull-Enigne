#ifndef __AUDIOEMITTER_H__
#define __AUDIOEMITTER_H__

#include "ModuleComponent.h"
#include "Globals.h"
#include "WwiseT.h"
#include "Timer.h"

class AudioEmitter : public Component
{
public:
	AudioEmitter(GameObject* parent);
	~AudioEmitter();
public:

	void Update(float dt);
	void ChangeVolume(float new_volume);
	void Mute(bool mute);
	void ChangeTimeToSwap(float new_time);
	void StartSound();
	void UpdateSourcePos();
	void SaveEmitter(JSON_Array* componentsObj) const;
	void LoadEmitter(JSON_Object* obj, GameObject* go);


public:
	WwiseT::AudioSource* source;
	std::string audio_name;
	float volume = 1.0f;
	float time_to_swap = 30.0f;
	int song = 2;
	bool mute = false;
	Timer timer;
	
	

};

#endif