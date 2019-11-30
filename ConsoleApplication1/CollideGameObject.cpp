#include "CollideGameObject.h"

void CollideGameObject::SetCollisionData()
{
	col.colRadius = m_scale;
	col.pos = m_position;
	col.offset = m_posOffset;
}

void CollideGameObject::UpdateCollisionPosition()
{
	col.pos = m_position;
}

void CollideGameObject::DrawDebug(Color & color)
{
	Collision::DrawCollision(col, color);
}

void CollideGameObject::Update(double elapsedTime)
{
	GameObject::Update();
	UpdateCollisionPosition();
}

bool CollideGameObject::CheckCollision(CollideGameObject * other)
{
	bool collided = Collision::CheckCollision(&col, &other->col);
	if (collided == true)
	{
		return true;
	}

	return false;
}

void CollideGameObject::UpdateFromNetwork(double elapsedTime)
{
}

bool Projectile::CheckCollision(CollideGameObject * other)
{
	bool collided = Collision::CheckCollision(&col, &other->col);
	if (collided == true)
	{
		bulletHealth -= 1.0f;
		return true;
	}

	return false;
}

void Projectile::SetCollisionData()
{
	col.colRadius = m_scale;
	col.pos = m_position;
	col.offset = m_posOffset;
}

void Projectile::Update(double elapsedTime)
{
	m_velocity += m_acceleration * elapsedTime;
	m_position += m_velocity * elapsedTime;
	UpdateCollisionPosition();
}

void Projectile::UpdateFromNetwork(double elapsedTime)
{
	m_velocity += m_acceleration * elapsedTime;
	m_position += m_velocity * elapsedTime;
	UpdateCollisionPosition();
}

void Projectile::Draw()
{
	GameObject::Draw();
	Sprite::DrawCircleLine(m_position.x, m_position.y, col.colRadius.xScale, col.colRadius.yScale, 300, m_color);
}

void Projectile::SetRotationToTarget(double elapsedTime)
{
	Vector3 tp = m_position;
	Vector3 tmp = targetPosition;

	Vector3 delta = tmp - tp;
	delta.Normalize();
	rotVector = delta;
	float rad = TWO_PI - (PI - atan2(delta.y, delta.x)); // In radians
	float deg = 90 + rad * (180 / PI);
	float degToRad = deg * PI / 180;
	SetRotation(degToRad);
}

void Projectile::UpdateRotation()
{
	Vector3 delta = rotVector;
	delta.Normalize();
	rotVector = delta;
	float rad = TWO_PI - (PI - atan2(delta.y, delta.x)); // In radians
	float deg = 90 + rad * (180 / PI);
	float degToRad = deg * PI / 180;
	SetRotation(degToRad);
}

void Missile::SetCollisionData()
{
	Scale temp = m_scale;
	temp.xScale *= 0.5f;
	col.colRadius = temp;
	col.pos = m_position;
	col.offset = m_posOffset;
}

void Missile::Update(double elapsedTime)
{
	SetRotationToTarget(elapsedTime);
	m_velocity = rotVector * speed;
	m_velocity += m_acceleration * elapsedTime;
	m_position += m_velocity * elapsedTime;
	UpdateCollisionPosition();
}

void Missile::Draw()
{
	GameObject::Draw();
}

void PlayerObject::Reset()
{
	playerData.health = playerData.maxHealth;
	playerData.score = 0;
}

void PlayerObject::HurtReset()
{
	m_color = defaultColor;
	hurtTimer = 0.3f;
	playerState = PlayerState::NONE;
}

void PlayerObject::InitializePlayerObject()
{
	Reset();
	HurtReset();
	if (playerData.playerType == PlayerType::PLAYER_1)
	{
		Vector3 pos = Vector3(17 * 4, 17 * 4, 0.0f);
		SetPos(pos);
		m_color = Color::Green;
		defaultColor = Color::Green;
		col.colShape = CollisionShape::CIRCLE;
		SetColor(m_color);
		SetCollisionData();
	}
	else if (playerData.playerType == PlayerType::PLAYER_2)
	{
		Vector3 pos = Vector3(15 * 78.3f, 15 * 58.3, 0.0f);
		SetPos(pos);
		m_color = Color::Yellow;
		defaultColor = Color::Yellow;
		col.colShape = CollisionShape::CIRCLE;
		SetColor(m_color);
		SetCollisionData();
	}

}

void PlayerObject::Restart()
{
	HurtReset();
	playerData.health = playerData.maxHealth;
	if (playerData.playerType == PlayerType::PLAYER_1)
	{
		Vector3 pos = Vector3(17 * 4, 17 * 4, 0.0f);
		SetPos(pos);
		m_color = Color::Green;
		defaultColor = Color::Green;
		col.colShape = CollisionShape::CIRCLE;
		SetColor(m_color);
		SetCollisionData();
	}
	else if (playerData.playerType == PlayerType::PLAYER_2)
	{
		Vector3 pos = Vector3(15 * 78.3f, 15 * 58.3, 0.0f);
		SetPos(pos);
		m_color = Color::Yellow;
		defaultColor = Color::Yellow;
		col.colShape = CollisionShape::CIRCLE;
		SetColor(m_color);
		SetCollisionData();
	}
}

void PlayerObject::SetCollisionData()
{
	col.colRadius = m_scale;
	col.pos = m_position;
	col.offset = m_posOffset;
}

void PlayerObject::Update(double elapsedTime)
{

	if (playerState == PlayerState::HURT)
	{
		hurtTimer -= elapsedTime;
		m_color = Color::Red;
		if (hurtTimer <= 0.0f)
		{
			m_color = defaultColor;
			hurtTimer = 0.3f;
			playerState = PlayerState::NONE;
		}
	}

	m_velocity += m_acceleration * elapsedTime;
	if (m_acceleration == Vector3(0.0f, 0.0f, 0.0f))
	{
		m_velocity *= 0.9999f;
	}

	m_position += m_velocity * elapsedTime;
	UpdateCollisionPosition();
}

void PlayerObject::SetRotationToTarget(double elapsedTime)
{
	Vector3 tp = m_position;
	Vector3 tmp = targetPosition;

	Vector3 delta = tmp - tp;
	delta.Normalize();
	rotVector = delta;
	float rad = TWO_PI - (PI - atan2(delta.y, delta.x)); // In radians
	float deg = 90 + rad * (180 / PI);
	float degToRad = deg * PI / 180;
	SetRotation(degToRad);
}

void PlayerObject::UpdateFromNetwork(double elapsedTime)
{
	Vector3 rotDirection = rotVector * 9000.0f + m_lastRotVector * 1000.0f;
	Vector3 delta = Vector3::Lerp(rotVector, rotDirection, elapsedTime);
	delta.Normalize();
	rotVector = delta;

	float rad = TWO_PI - (PI - atan2(delta.y, delta.x)); // In radians
	float deg = 90 + rad * (180 / PI);
	float degToRad = deg * PI / 180;
	SetRotation(degToRad);

	Update(elapsedTime);
	SetPos(GetPos()*0.995f + m_lastReceivedPos * 0.005f);
	UpdateCollisionPosition();
}

void PlayerObject::UpdateRotation()
{
	Vector3 delta = rotVector;
	delta.Normalize();
	rotVector = delta;
	float rad = TWO_PI - (PI - atan2(delta.y, delta.x)); // In radians
	float deg = 90 + rad * (180 / PI);
	float degToRad = deg * PI / 180;
	SetRotation(degToRad);
}

bool PlayerObject::CheckCollision(CollideGameObject * other, double elapsedTime)
{
	Vector3 tempVel = m_velocity + m_acceleration * elapsedTime;
	Vector3 tempPos = m_position + tempVel * elapsedTime;
	Collision tempCol = col;
	tempCol.pos = tempPos;

	bool collided = Collision::CheckCollision(&col, &other->col);
	if (collided == true)
	{
		if (other->m_gameObjectTag == GameObjectTag::OBSTACLE_WALL)
		{
			SetAccel(Vector3(0.0f, 0.0f, 0.0f));
			//SetVel(Vector3(0.0f, 0.0f, 0.0f));
			SetVel(m_velocity * -1.0f);
		}

		return true;
	}

	return false;
}

void PlayerObject::TakeDamage(float damage)
{
	playerState = PlayerState::HURT;
	playerData.health -= damage;
	if (playerData.health < 0.0f)
	{
		playerData.health = 0.0f;
	}
}

Color & PlayerObject::GetDefaultColor()
{
	return defaultColor;
}

void PlayerObject::Draw()
{
	GameObject::Draw();
	Sprite::DrawCircleLine(m_position.x, m_position.y, col.colRadius.xScale, col.colRadius.yScale, 300, m_color);
}
