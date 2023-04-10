#pragma once
#include<SDL_mixer.h>

class Music
{
public:

	Music() :
		music_{ nullptr },
		chunk_{ nullptr }
	{

		// 初始化 SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

		}

	}

	Music(const char* musicpath) :
		music_{ nullptr },
		chunk_{ nullptr }
	{

		// 初始化 SDL_mixer
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;

		}

		// 加载音乐
		setMusic(musicpath);

	}

	~Music()
	{
		// 释放资源
		Mix_FreeMusic(music_);
		Mix_FreeChunk(chunk_);

		Mix_CloseAudio();

	}

	// 循环播放音乐
	void playMusic()
	{
		if (Mix_PlayingMusic())
			return;

		// 播放音频
		if (Mix_PlayMusic(music_, -1) == -1)
		{
			std::cout << "Mix_PlayMusic Error: " << Mix_GetError() << std::endl;
			return;
		}

	}

	// 只播放一次音效
	void playOnce(const int& n = 1, const bool& retur = false)
	{
		if (retur&&Mix_Playing(n))
			return;

		// 播放音频
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

		// 加载音乐
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

		// 加载音效
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

	// 音乐资源指针
	Mix_Music* music_;

	// 音效文件
	Mix_Chunk* chunk_;

};
