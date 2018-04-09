#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )


ModuleAudio::ModuleAudio() 
{
	for (uint i = 0; i < MAX_MUSIC;++i) 
	{
		mus[i] = nullptr;
	}
	for (uint i = 0; i < MAX_FX;++i)
	{
		sound_fx[i] = nullptr;
	}
};
ModuleAudio::~ModuleAudio() {};


bool ModuleAudio::Init() 
{
	
	bool ret = true;
	LOG("Audio module init");
	if (Mix_Init(MIX_INIT_OGG)<0)
	{
		LOG("Audio module can't be initialitzated");
		ret = false;
	}
	else
	{
		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024)<0) 
		{
			LOG("Audio module can't open %s",Mix_GetError());
			ret = false;
		}
	}
	return ret;
}
bool ModuleAudio::CleanUp() 
{
	for (int i = 0; i < MAX_MUSIC; ++i) 
	{
		if (mus[i] != nullptr)
		{
			Mix_FreeMusic(mus[i]);
		}
	}
	for (uint i = 0; i < MAX_MUSIC; ++i)
	{
		if (sound_fx[i] != nullptr)
		{
			Mix_FreeChunk(sound_fx[i]);
		}
	}

	Mix_CloseAudio();
	Mix_Quit();

	return true;
}

void ModuleAudio::PlayMusic(Mix_Music* mus_to_play,int reps) 
{
	Mix_PlayMusic(mus_to_play,reps);
	Mix_VolumeMusic(MIX_MAX_VOLUME/3);
}
void ModuleAudio::PlaySoundEffects(Mix_Chunk* fx_to_play)
{
	Mix_PlayChannel (-1,fx_to_play,0);
	Mix_VolumeChunk(fx_to_play, MIX_MAX_VOLUME/ 3);
}
void ModuleAudio::UnloadMusic(Mix_Music * mus)
{
	for (uint i = 0; i < last_song; ++i) 
	{
		if (this->mus[i] == mus)
		{
			Mix_FreeMusic(mus);
		}
	}
}
void ModuleAudio::UnloadFx(Mix_Chunk * fx)
{
	for (uint i = 0; i < last_fx; ++i)
	{
		if (sound_fx[i] == fx)
		{
			Mix_FreeChunk(fx);
		}
	}
}

Mix_Music* const ModuleAudio::LoadMusic(const char* path)
{
	Mix_Music* music = nullptr;
	if (last_song < MAX_MUSIC)
	{
		music = Mix_LoadMUS(path);

		if (mus == nullptr)
		{
			LOG("Could not load sound with path %s Error: %s", path, Mix_GetError());
		}
		else
		{
			mus[last_song++] = music;
		}
	}
	return music;
}
Mix_Chunk* const ModuleAudio::LoadEffect(const char* path)
{
	Mix_Chunk* fx = nullptr;
	if (last_fx < MAX_FX)
	{
		fx = Mix_LoadWAV(path);

		if (fx == nullptr)
		{
			LOG("Could not load effect with path %s Error: %s", path, Mix_GetError());
		}
		else
		{
			sound_fx[last_fx++] = fx;
		}
	}
	return fx;
}