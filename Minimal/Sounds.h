#ifndef H_Sounds
#define H_Sounds
#pragma once

#include "al.h" 
#include "alc.h"
#include "alut.h"
#include <fstream>
#include <cstring>


#ifndef AL_CHECK
#ifdef _DEBUG
#define AL_CHECK(stmt) do { \
            stmt; \
            CheckOpenALError(#stmt, __FILE__, __LINE__); \
        } while (0);
#else
#define AL_CHECK(stmt) stmt
#endif
#endif

class Sounds {
public:
	ALCdevice * deviceAL;
	ALCcontext * contextAL;
	ALuint source;
	ALuint sndBuffer;
	ALint source_state;
	unsigned int sourceid;
	Sounds(const char* fn);
	void Play();
	~Sounds();

private:
	
};

#endif

