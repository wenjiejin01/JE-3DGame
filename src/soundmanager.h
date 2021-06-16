#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "utils.h"
#include <iostream>
#include <vector>
#include "framework.h"
#include <map>
#include "extra/bass.h"


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager* instance;

	static SoundManager* getInstance() {
		if (instance == NULL) instance = new SoundManager();
		return instance;
	}

	void playSound(const std::string& name, bool loop);
	void StopSound(const std::string& name);
	void SetVolume(const std::string& name, float value);
	bool IsStarted(const std::string& name);

	std::map<std::string, HCHANNEL> channels;
	std::map<std::string, HSAMPLE> samples;
};

#endif	

