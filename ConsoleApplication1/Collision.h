#pragma once
#include "AllHeader.h"
#include "Sprite.h"
#include "Transformation.h"
#include "Vector3.h"
#include "Color.h"

enum class CollisionShape
{
	NONE,
	SQUARE,
	CIRCLE
};

class Collision
{
private:
	static bool CheckRectangleCircleCollision(Collision* rect, Collision* circle);	
	static bool CheckRectangleCollision(Collision* rect1, Collision* rect2);
	static bool CheckCircleCollision(Collision* circle1, Collision* circle2);
	static void DrawRectangleCollision(Collision rect, Color& color);
	static void DrawCircleCollision(Collision circle, Color& color);

public:
	CollisionShape colShape = CollisionShape::SQUARE;
	Vector3 pos;
	Scale colRadius;
	MatrixOffset offset;

	static bool CheckCollision(Collision* col1, Collision* col2);
	static void DrawCollision(Collision col, Color& color);
};