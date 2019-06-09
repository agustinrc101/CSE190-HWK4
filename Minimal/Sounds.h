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
	ALuint source2;
	ALuint source3;
	ALuint source4;
	ALuint sndBuffer;
	ALuint sndBuffer2;
	ALuint sndBuffer3;
	ALuint sndBuffer4;
	ALint source_state;
	ALint source_state2;
	ALint source_state3;
	ALint source_state4;
	unsigned int sourceid;
	unsigned int sourceid2;
	unsigned int sourceid3;
	unsigned int sourceid4;
	Sounds(const char* fn);
	void Play(float x, float y, float z, int sourceid);
	~Sounds();

private:
	
};

#endif

