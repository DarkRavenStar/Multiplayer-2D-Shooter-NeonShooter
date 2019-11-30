#pragma once
#include "GameObject.h"

class CollideGameObject : public GameObject
{
public:
	Collision col;
	Vector3 rotVector = Vector3();

	CollideGameObject(Sprite* sprite) : GameObject(sprite) {}
	CollideGameObject() {}

	virtual void SetCollisionData();
	virtual void UpdateCollisionPosition();
	virtual bool CheckCollision(CollideGameObject* other);
	virtual void DrawDebug(Color& color);
	virtual void Update(double elapsedTime) override;
	virtual void UpdateFromNetwork(double elapsedTime);
};

class BoundaryObject : public CollideGameObject
{
public:

	BoundaryObject(Sprite* sprite) : CollideGameObject(sprite)
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE;
	}
	
	BoundaryObject()
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE;
	}
};

class Wall : public BoundaryObject
{
public:
	Wall()
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE_WALL;
		col.colShape = CollisionShape::SQUARE;
	}


	Wall(Sprite* sprite) : BoundaryObject(sprite)
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE_WALL;
		col.colShape = CollisionShape::SQUARE;
	}
};

class CircularWall : public Wall
{
public:
	CircularWall()
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE_WALL;
		col.colShape = CollisionShape::CIRCLE;
	}


	CircularWall(Sprite* sprite) : Wall(sprite)
	{
		m_gameObjectTag = GameObjectTag::OBSTACLE_WALL;
		col.colShape = CollisionShape::CIRCLE;
	}
};

class Projectile : public CollideGameObject
{
public:
	float speed;
	float bulletHealth = 1.0f;
	float bulletDamage = 1.0f;
	Vector3 targetPosition;
	Vector3 m_lastReceivedPos;
	Vector3 m_lastKnownPos;
	Vector3 m_lastRotVector;

	Projectile()
	{
		m_gameObjectTag = GameObjectTag::BULLET;
		col.colShape = CollisionShape::CIRCLE;
	}

	Projectile(Sprite* sprite) : CollideGameObject(sprite)
	{
		m_gameObjectTag = GameObjectTag::BULLET;
		col.colShape = CollisionShape::CIRCLE;
	}

	virtual bool CheckCollision(CollideGameObject* other);
	virtual void SetCollisionData() override;
	virtual void Update(double elapsedTime) override;
	virtual void UpdateFromNetwork(double elapsedTime) override;
	virtual void Draw() override;
	void SetRotationToTarget(double elapsedTime);
	void UpdateRotation();
	
};

class Bullet : public Projectile
{
public:
	Bullet()
	{
		m_gameObjectTag = GameObjectTag::BULLET;
		col.colShape = CollisionShape::CIRCLE;
		bulletHealth = 1.0f;
		bulletDamage = 1.0f;
	}

	Bullet(Sprite* sprite) : Projectile(sprite)
	{
		m_gameObjectTag = GameObjectTag::BULLET;
		col.colShape = CollisionShape::CIRCLE;
		bulletHealth = 1.0f;
		bulletDamage = 1.0f;
	}
};

class Missile : public Projectile
{
public:
	Missile()
	{
		m_gameObjectTag = GameObjectTag::MISSILE;
		col.colShape = CollisionShape::CIRCLE;
		bulletHealth = 10.0f;
		bulletDamage = 2.0f;
	}


	Missile(Sprite* sprite) : Projectile(sprite)
	{
		m_gameObjectTag = GameObjectTag::MISSILE;
		col.colShape = CollisionShape::CIRCLE;
		bulletHealth = 10.0f;
		bulletDamage = 2.0f;
	}

	virtual void SetCollisionData() override;
	virtual void Update(double elapsedTime) override;
	virtual void Draw() override;
	
};

class ProjectileObject
{
public:
	bool sent = false;
	int m_ID = -1;
	Projectile m_Bullet;

	ProjectileObject(int id, Projectile bullet)
	{
		m_ID = id;
		m_Bullet = bullet;
	}
};

class MissileObject
{
public:
	bool sent = false;
	int m_ID = -1;
	Missile m_Bullet;

	MissileObject(int id, Missile bullet)
	{
		m_ID = id;
		m_Bullet = bullet;
	}
};

class BulletObject
{
public:
	bool sent = false;
	int m_ID = -1;
	Bullet m_Bullet;

	BulletObject(int id, Bullet bullet)
	{
		m_ID = id;
		m_Bullet = bullet;
	}
};

enum class PlayerState
{
	NONE,
	HURT,
};

class PlayerObject : public CollideGameObject
{
private:
	float rotVelocity;
	Color defaultColor;
	PlayerState playerState;
	Bullet normalBulletTemplate;
	Missile missileTemplate;
	float hurtTimer = 0.3f;

	void Reset();
	void HurtReset();
	

public:
	PlayerData playerData;
	Vector3 targetPosition;
	Vector3 m_lastReceivedPos;
	Vector3 m_lastRotVector;

	PlayerObject()
	{
		m_gameObjectTag = GameObjectTag::PLAYER;
	}
	PlayerObject(Sprite* sprite) : CollideGameObject(sprite)
	{
		m_gameObjectTag = GameObjectTag::PLAYER;
	}

	void InitializePlayerObject();
	void Restart();
	virtual void SetCollisionData() override;
	virtual void Update(double elapsedTime) override;
	void SetRotationToTarget(double elapsedTime);
	virtual void UpdateFromNetwork(double elapsedTime) override;
	void UpdateRotation();
	virtual bool CheckCollision(CollideGameObject* other, double elapsedTime);
	void TakeDamage(float damage);
	Color& GetDefaultColor();
	virtual void Draw() override;
	
};