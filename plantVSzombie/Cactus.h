#pragma once
#pragma once
#include"attackPlant.h"
#include"Bullet.h"

namespace cactusTEMP
{
	// 子弹动画
	static const char* bulletPath{ "E:\\GamePicture\\image\\Bullet\\cactusBullet" };

	// 子弹发射音效
	static const char* bulletMusicPath{ "E:\\GamePicture\\Music\\swing.ogg" };

	// 植物动画
	static const char* plantPath{ "E:\\GamePicture\\image\\cactus\\Cactus\\Frame" };

	// 击中动画
	static const char* attackedPath{ "E:\\GamePicture\\image\\Bullet\\PeaBullet1.bmp" };

	// 击中音效
	static const char* attackedMusicPath{ "E:\\GamePicture\\Music\\kernelpult.ogg" };

};

class cactus final : public attackPlant
{


public:

	cactus(int x, int y) :
		attackPlant(x, y)
	{
		// 子弹音效
		bullet_ = std::shared_ptr<Bullet>{ new Bullet(cactusTEMP::bulletMusicPath) };

		// 设置子弹位置
		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		// 植物动画
		setPath(cactusTEMP::plantPath, ActMode::Wait);

		// 设置子弹动画
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		// 设置子弹攻击力
		bullet_->setFire(8);

		// 击中僵尸的音效
		music_.setChunk(cactusTEMP::attackedMusicPath);

	}

	cactus(const Position& pos) :
		attackPlant(pos)
	{
		// 子弹音效
		bullet_ = std::shared_ptr<Bullet>{ new Bullet(cactusTEMP::bulletMusicPath) };

		// 设置子弹位置
		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		// 植物动画
		setPath(cactusTEMP::plantPath, ActMode::Wait);

		// 设置子弹动画
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		// 设置子弹攻击力
		bullet_->setFire(8);

		// 击中僵尸的音效
		music_.setChunk(cactusTEMP::attackedMusicPath);
	}


	~cactus() = default;

	// 设置植物等待动画和 子弹动画个数
	virtual void playImage(SDL_Renderer* render, bool startPlay)override
	{

		// 播放植物动画
		waitAction(render, 10);

		if (startPlay)
			bullet_->headAttackAction(render, 1);
	}


	virtual void waitAction(SDL_Renderer* render, const int& max)override
	{
		Action::waitAction(render, max);
	}


	int demage()override
	{
		return 1;
	}

	int getSunPrice()override
	{
		return 1;
	}

	// 重载重装子弹射击
	bool shoot(SDL_Renderer* render)override
	{
		// 设置子弹音效
		bullet_ = std::shared_ptr<Bullet>{ new Bullet{cactusTEMP::bulletMusicPath} };

		// 设置子弹动画
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		bullet_->setFire(8);

		return false;
	}

	// 设置子弹击中僵尸的画面
	virtual SDL_Texture* setAttackedTexture(SDL_Renderer* render) override
	{
		// 加载击中的动画
		auto temp = SDL_LoadBMP(cactusTEMP::attackedPath);

		// 设置透明
		SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 255, 255));

		auto Texture_ = SDL_CreateTextureFromSurface(render, temp);

		SDL_FreeSurface(temp);

		return Texture_;
	}

	// 播放击中僵尸的音效和画面
	void player(SDL_Renderer* render, SDL_Texture* texture)
	{
		//SDL_Rect rect{ bullet_->getPos().x_  ,bullet_->getPos().y_,0,0 };

		//if (texture)
		//	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

		//// 渲染画面
		//SDL_RenderCopy(render, texture, nullptr, &rect);


		music_.playOnce(4);
	}


private:
	// 击中僵尸的音效
	Music music_;
};
