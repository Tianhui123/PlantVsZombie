#pragma once
#pragma once
#include"attackPlant.h"
#include"Bullet.h"

namespace cactusTEMP
{
	// �ӵ�����
	static const char* bulletPath{ "E:\\GamePicture\\image\\Bullet\\cactusBullet" };

	// �ӵ�������Ч
	static const char* bulletMusicPath{ "E:\\GamePicture\\Music\\swing.ogg" };

	// ֲ�ﶯ��
	static const char* plantPath{ "E:\\GamePicture\\image\\cactus\\Cactus\\Frame" };

	// ���ж���
	static const char* attackedPath{ "E:\\GamePicture\\image\\Bullet\\PeaBullet1.bmp" };

	// ������Ч
	static const char* attackedMusicPath{ "E:\\GamePicture\\Music\\kernelpult.ogg" };

};

class cactus final : public attackPlant
{


public:

	cactus(int x, int y) :
		attackPlant(x, y)
	{
		// �ӵ���Ч
		bullet_ = std::shared_ptr<Bullet>{ new Bullet(cactusTEMP::bulletMusicPath) };

		// �����ӵ�λ��
		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		// ֲ�ﶯ��
		setPath(cactusTEMP::plantPath, ActMode::Wait);

		// �����ӵ�����
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		// �����ӵ�������
		bullet_->setFire(8);

		// ���н�ʬ����Ч
		music_.setChunk(cactusTEMP::attackedMusicPath);

	}

	cactus(const Position& pos) :
		attackPlant(pos)
	{
		// �ӵ���Ч
		bullet_ = std::shared_ptr<Bullet>{ new Bullet(cactusTEMP::bulletMusicPath) };

		// �����ӵ�λ��
		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		// ֲ�ﶯ��
		setPath(cactusTEMP::plantPath, ActMode::Wait);

		// �����ӵ�����
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		// �����ӵ�������
		bullet_->setFire(8);

		// ���н�ʬ����Ч
		music_.setChunk(cactusTEMP::attackedMusicPath);
	}


	~cactus() = default;

	// ����ֲ��ȴ������� �ӵ���������
	virtual void playImage(SDL_Renderer* render, bool startPlay)override
	{

		// ����ֲ�ﶯ��
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

	// ������װ�ӵ����
	bool shoot(SDL_Renderer* render)override
	{
		// �����ӵ���Ч
		bullet_ = std::shared_ptr<Bullet>{ new Bullet{cactusTEMP::bulletMusicPath} };

		// �����ӵ�����
		bullet_->setPath(cactusTEMP::bulletPath, ActMode::HeadAttack);

		bullet_->setPos(this->pos_.x_ + 40, this->pos_.y_+30);

		bullet_->setFire(8);

		return false;
	}

	// �����ӵ����н�ʬ�Ļ���
	virtual SDL_Texture* setAttackedTexture(SDL_Renderer* render) override
	{
		// ���ػ��еĶ���
		auto temp = SDL_LoadBMP(cactusTEMP::attackedPath);

		// ����͸��
		SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 255, 255));

		auto Texture_ = SDL_CreateTextureFromSurface(render, temp);

		SDL_FreeSurface(temp);

		return Texture_;
	}

	// ���Ż��н�ʬ����Ч�ͻ���
	void player(SDL_Renderer* render, SDL_Texture* texture)
	{
		//SDL_Rect rect{ bullet_->getPos().x_  ,bullet_->getPos().y_,0,0 };

		//if (texture)
		//	SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);

		//// ��Ⱦ����
		//SDL_RenderCopy(render, texture, nullptr, &rect);


		music_.playOnce(4);
	}


private:
	// ���н�ʬ����Ч
	Music music_;
};
