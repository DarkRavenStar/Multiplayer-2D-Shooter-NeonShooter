#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"
#include "Collision.h"
#include "GameObjectTag.h"

enum class BlendMode
{
	NONE,
	ADDITIVE,
	ADDITIVE_ALPHA,
	MULTIPLICATIVE,
	ALPHA
};

class GameObject
{
protected:
	Sprite* m_sprites = NULL;
	Vector3 m_position = Vector3();
	Vector3 m_velocity = Vector3();
	Vector3 m_acceleration = Vector3();
	MatrixOffset m_posOffset = MatrixOffset();
	float m_rotation = 0.0f;
	Scale m_scale;
	Color m_color;
	BlendMode m_blendMode = BlendMode::ADDITIVE_ALPHA;
public:
	GameObjectTag m_gameObjectTag = GameObjectTag::NONE;
	GameObject();
	GameObject(Sprite* sprite);

	void SetPos(Vector3 pos);
	Vector3& GetPos();

	void SetVel(Vector3 vel);
	Vector3& GetVel();

	void SetAccel(Vector3 accel);
	Vector3& GetAccel();

	void SetPosOffset(MatrixOffset offset);
	MatrixOffset& GetPosOffset();

	virtual void SetRotation(float rotate);
	float& GetRotation();
	
	void SetScale(float scaleX, float scaleY);
	Scale& GetScale();
	
	void SetSprite(Sprite* sprite);
	Sprite* GetSprite();

	void SetBlendMode(BlendMode blendMode)
	{
		m_blendMode = blendMode;
	}

	BlendMode& GetBlendMode()
	{
		return m_blendMode;
	}
	
	void SetColor(Color& color);
	Color& GetColor();
	
	virtual void Draw();
	virtual void Update();
	virtual void Update(double elapsedTime);
	virtual void Collided();
};