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
		}
		else {
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


Sounds::Sounds(const char* filename) {


	//
	int channel, sampleRate, bps, size;
	int channel2, sampleRate2, bps2, size2;
	char* data = loadWAV(filename, channel, sampleRate, bps, size);
	char* data2 = loadWAV("Sound/SDR2_OST_-_-2-03-_Beautiful_Ruin-vNYydvw13LM.wav", channel2, sampleRate2, bps2, size2);
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
	unsigned int bufferid2, format2;
	alGenBuffers(1, &bufferid);
	alGenBuffers(1, &bufferid2);
	format = getFormat(channel, bps);
	format2 = getFormat(channel2, bps2);
	alBufferData(bufferid, format, data, size, sampleRate);
	alBufferData(bufferid2, format2, data2, size2, sampleRate2);

	alGenSources(1, &sourceid);
	alGenSources(1, &sourceid2);
	alSourcei(sourceid, AL_BUFFER, bufferid);
	alSourcei(sourceid2, AL_BUFFER, bufferid2);
	if ((alGetError()) != AL_NO_ERROR)
	{
		std::cout << "alSourceasdf;lasdjf;alsjdf" << std::endl;

	}
	alSourcePlay(sourceid2);
	while (source_state2 == AL_PLAYING) {
		alGetSourcei(source2, AL_SOURCE_STATE, &source_state2);
	}


};

void Sounds::Play() {
	alSourcePlay(sourceid);

	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	}
}

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
		std::cout << ("OpenAL error %08x, (%s) at %s:%i - for %s", err, GetOpenALErrorString(err), fname, line, stmt);
	}
};