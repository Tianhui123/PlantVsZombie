#pragma once
#include<SDL_mixer.h>

class Music
{
public:

	Music() :
		music_{ nullptr },
		chunk_{ nullptr }
	{

		// ��ʼ�� SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

		}

	}

	Music(const char* musicpath) :
		music_{ nullptr },
		chunk_{ nullptr }
	{

		// ��ʼ�� SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

		}

		// ��������
		setMusic(musicpath);

	}

	~Music()
	{
		// �ͷ���Դ
		Mix_FreeMusic(music_);
		Mix_FreeChunk(chunk_);

		Mix_CloseAudio();

	}

	// ѭ����������
	void playMusic()
	{
		if (Mix_PlayingMusic())
			return;

		// ������Ƶ
		if (Mix_PlayMusic(music_, -1) == -1)
		{
			std::cout << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
			return;
		}

	}

	// ֻ����һ����Ч
	void playOnce(const int& n = 1, const bool& retur = false)
	{
		if (retur&&Mix_Playing(n))
			return;

		// ������Ƶ
		if (Mix_PlayChannel(n, chunk_, 0) == -1)
		{
			std::cout << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
			return;
		}

	}

	void pause(bool p)
	{
		Mix_PauseMusic();
	}

	void resume()
	{
		Mix_ResumeMusic();
	}

	void setMusic(const char* path)
	{
		if (music_)
			Mix_FreeMusic(music_);

		// ��������
		music_ = Mix_LoadMUS(path);
		if (!music_)
		{
			std::cout << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
		}

	}

	void setChunk(const char* path)
	{

		if (chunk_)
			Mix_FreeChunk(chunk_);

		// ������Ч
		chunk_ = Mix_LoadWAV(path);
		if (!music_)
		{
			std::cout << "Mix_LoadWAV Error: " << Mix_GetError() << std::endl;
		}

	}

	bool isChunkNULL()
	{
		return chunk_ == nullptr;
	}

	bool isMusicNULL()
	{
		return music_ == nullptr;
	}

private:

	// ������Դָ��
	Mix_Music* music_;

	// ��Ч�ļ�
	Mix_Chunk* chunk_;

};
