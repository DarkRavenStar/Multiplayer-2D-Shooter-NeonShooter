#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(Sprite* sprite)
{
	m_sprites = sprite;
}

void GameObject::SetPos(Vector3 pos)
{
	m_position = pos;
}

Vector3& GameObject::GetPos()
{
	return m_position;
}

void GameObject::SetVel(Vector3 vel)
{
	m_velocity = vel;
}

Vector3 & GameObject::GetVel()
{
	return m_velocity;
}

void GameObject::SetAccel(Vector3 accel)
{
	m_acceleration = accel;
}

Vector3 & GameObject::GetAccel()
{
	return m_acceleration;
}

void GameObject::SetPosOffset(MatrixOffset offset)
{
	m_posOffset = offset;
}

MatrixOffset& GameObject::GetPosOffset()
{
	return m_posOffset;
}

void GameObject::SetRotation(float rotate)
{
	m_rotation = rotate;
}

float& GameObject::GetRotation()
{
	return m_rotation;
}

void GameObject::SetScale(float scaleX, float scaleY)
{
	m_scale.xScale = scaleX;
	m_scale.yScale = scaleY;
}

Scale& GameObject::GetScale()
{
	return m_scale;
}

void GameObject::SetSprite(Sprite* sprite)
{
	m_sprites = sprite;
}

Sprite* GameObject::GetSprite()
{
	return m_sprites;
}

void GameObject::SetColor(Color& color)
{
	m_color = color;
}

Color& GameObject::GetColor()
{
	return m_color;
}

void GameObject::Draw()
{
	if (m_sprites != NULL)
	{
		if (m_blendMode != BlendMode::NONE)
		{
			glEnable(GL_BLEND);

			if (m_blendMode == BlendMode::ADDITIVE)
			{
				//Without Alpha
				glBlendFunc(GL_ONE, GL_ONE);
			}
			else if (m_blendMode == BlendMode::ADDITIVE_ALPHA)
			{
				//With Alpha
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			}
			else if (m_blendMode == BlendMode::MULTIPLICATIVE)
			{
				//glBlendFunc(GL_DST_COLOR, GL_ONE);
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
			}
			else if (m_blendMode == BlendMode::ALPHA)
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
		m_sprites->Draw(m_position, m_posOffset, m_rotation, m_scale.xScale, m_scale.yScale, m_color);
		glDisable(GL_BLEND);
	}
}

void GameObject::Update()
{
}

void GameObject::Update(double elapsedTime)
{
	m_velocity += m_acceleration * elapsedTime;
	m_position += m_velocity * elapsedTime;
}

void GameObject::Collided()
{
}
