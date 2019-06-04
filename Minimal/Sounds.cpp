#include "Sounds.h"
#include <iostream>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096




bool isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i < len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}
// alutLoadWAVFile("test.wav", &format, &data, &size, &freq, &loop);
char* loadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0)
	{
		std::cout << "this is not a valid WAVE file" << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	in.read(buffer, 4);      //WAVE
	in.read(buffer, 4);      //fmt
	in.read(buffer, 4);      //16
	in.read(buffer, 2);      //1
	in.read(buffer, 2);
	chan = convertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertToInt(buffer, 2);
	in.read(buffer, 4);      //data
	in.read(buffer, 4);
	size = convertToInt(buffer, 4);
	char* data = new char[size];
	in.read(data, size);
	return data;
}


unsigned int getFormat(int channel, int bps) {
	if (channel == 1)
	{
		if (bps == 8)
		{
			return AL_FORMAT_MONO8;
		} else {
			return AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			return AL_FORMAT_STEREO8;
		}
		else {
			return AL_FORMAT_STEREO16;
		}
	}
}


Sounds::Sounds() {

	
	//alGetError();

	//deviceAL = alcOpenDevice(NULL);

	//if (deviceAL == NULL)
	//{
	//	std::cout << ("Failed to init OpenAL device.") << std::endl;
	//	return;
	//}

	//contextAL = alcCreateContext(deviceAL, NULL);
	//AL_CHECK(alcMakeContextCurrent(contextAL));

	//


	//alGenSources((ALuint)1, &source);
	//// check for errors

	//// check for errors

	//// ALsizei size, freq;
	//// ALenum format;
	//// ALvoid *data;
	//// ALboolean loop = AL_FALSE;

	//unsigned int bufferid, format;
	//
	//int chan, sampleRate, bps, size;
	//ALvoid *data = loadWAV("h.wav", chan, sampleRate, bps, size);
	//alGenBuffers(1, &sndBuffer);

	//alBufferData(sndBuffer, getFormat(chan, bps), data, size, sampleRate);
	//
	//////wave to buffer

	//alSourcei(source, AL_BUFFER, sndBuffer);

	//alSourcePlay(source);
	//// check for errors

	//alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//// check for errors
	//while (source_state == AL_PLAYING) {
	//	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	//	// check for errors
	//}
	int channel, sampleRate, bps, size;
	char* data = loadWAV("67032__robinhood76__00892-funeral-silencium-trumpet.wav", channel, sampleRate, bps, size);
	ALCdevice* device = alcOpenDevice(NULL);
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
		return;
	}
	ALCcontext* context = alcCreateContext(device, NULL);
	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
		return;
	}
	alcMakeContextCurrent(context);

	unsigned int bufferid, format;
	alGenBuffers(1, &bufferid);
	if (channel == 1)
	{
		if (bps == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else {
			format = AL_FORMAT_MONO16;
		}
	}
	else {
		if (bps == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else {
			format = AL_FORMAT_STEREO16;
		}
	}
	alBufferData(bufferid, format, data, size, sampleRate);
	unsigned int sourceid;
	alGenSources(1, &sourceid);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	if (( alGetError()) != AL_NO_ERROR)
	{
		std::cout<<"alSourceasdf;lasdjf;alsjdf" << std::endl;
		
	}
	alSourcePlay(sourceid);

	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
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