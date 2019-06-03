#include "Sounds.h"
#include <iostream>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096







Sounds::Sounds() {


	alGetError();

	deviceAL = alcOpenDevice(NULL);

	if (deviceAL == NULL)
	{
		std::cout << ("Failed to init OpenAL device.") << std::endl;
		return;
	}

	contextAL = alcCreateContext(deviceAL, NULL);
	AL_CHECK(alcMakeContextCurrent(contextAL));

	


	alGenSources((ALuint)1, &source);
	// check for errors

	alSourcef(source, AL_PITCH, 1);
	// check for errors

	ALsizei size, freq;
	ALenum format;
	ALvoid *data;
	ALboolean loop = AL_FALSE;
	
	
	
	////wave to buffer

	alSourcei(source, AL_BUFFER, sndBuffer);

	alSourcePlay(source);
	// check for errors

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	// check for errors
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		// check for errors
	}

};

Sounds::~Sounds() {
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &sndBuffer);
	deviceAL = alcGetContextsDevice(contextAL);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(contextAL);
	alcCloseDevice(deviceAL);
}

const char * GetOpenALErrorString(int errID)
{
	if (errID == AL_NO_ERROR) return "";
	if (errID == AL_INVALID_NAME) return "Invalid name";
	if (errID == AL_INVALID_ENUM) return " Invalid enum ";
	if (errID == AL_INVALID_VALUE) return " Invalid value ";
	if (errID == AL_INVALID_OPERATION) return " Invalid operation ";
	if (errID == AL_OUT_OF_MEMORY) return " Out of memory like! ";

	return " Don't know ";
}


inline void CheckOpenALError(const char* stmt, const char* fname, int line)
{

	ALenum err = alGetError();
	if (err != AL_NO_ERROR)
	{
		std::cout<<("OpenAL error %08x, (%s) at %s:%i - for %s", err, GetOpenALErrorString(err), fname, line, stmt);
	}
};