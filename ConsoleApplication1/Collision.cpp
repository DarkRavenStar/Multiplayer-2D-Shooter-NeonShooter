#include "Collision.h"

bool Collision::CheckRectangleCircleCollision(Collision * rect, Collision * circle)
{
	float rx = rect->pos.x + (rect->colRadius.xScale * rect->offset.offset[0].x);
	float rw = rect->pos.x + (rect->colRadius.xScale * rect->offset.offset[3].x);

	float ry = rect->pos.y + (rect->colRadius.yScale * rect->offset.offset[0].y);
	float rh = rect->pos.y + (rect->colRadius.yScale * rect->offset.offset[1].y);

	float rxs = rect->colRadius.xScale;
	float rys = rect->colRadius.yScale;

	// temporary variables to set edges for testing
	float cx = circle->pos.x;
	float cy = circle->pos.y;
	float cr = circle->colRadius.xScale;
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // top edge
	else if (cy > ry + rh) testY = ry + rh;   // bottom edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX*distX) + (distY*distY));

	// if the distance is less than the radius, collision!
	if (distance <= cr)
	{
		return true;
	}
	return false;
}

bool Collision::CheckRectangleCollision(Collision * rect1, Collision * rect2)
{
	float scaleX1 = rect1->colRadius.xScale;
	float scaleY1 = rect1->colRadius.yScale;

	float scaleX2 = rect2->colRadius.xScale;
	float scaleY2 = rect2->colRadius.yScale;

	float xMin1 = rect1->pos.x + (rect1->colRadius.xScale * rect1->offset.offset[0].x);
	float xMax1 = rect1->pos.x + (rect1->colRadius.xScale * rect1->offset.offset[3].x);

	float yMin1 = rect1->pos.y + (rect1->colRadius.yScale * rect1->offset.offset[0].y);
	float yMax1 = rect1->pos.y + (rect1->colRadius.yScale * rect1->offset.offset[1].y);

	float xMin2 = rect2->pos.x + (rect2->colRadius.xScale * rect2->offset.offset[0].x);
	float xMax2 = rect2->pos.x + (rect2->colRadius.xScale * rect2->offset.offset[3].x);

	float yMin2 = rect2->pos.y + (rect2->colRadius.yScale * rect2->offset.offset[0].y);
	float yMax2 = rect2->pos.y + (rect2->colRadius.yScale * rect2->offset.offset[1].y);

	if (xMin1 < xMax2 && xMax1 > xMin2 && yMin1 < yMax2 && yMax1 > yMin2)
	{
		return true;
	}

	return false;
}

bool Collision::CheckCircleCollision(Collision * circle1, Collision * circle2)
{
	float c1x = circle1->pos.x;
	float c1y = circle1->pos.y;
	float c1r = circle1->colRadius.xScale;

	float c2x = circle2->pos.x;
	float c2y = circle2->pos.y;
	float c2r = circle2->colRadius.xScale;

	// get distance between the circle's centers
	// use the Pythagorean Theorem to compute the distance
	float distX = c1x - c2x;
	float distY = c1y - c2y;
	float distance = sqrt((distX*distX) + (distY*distY));

	// if the distance is less than the sum of the circle's
	// radii, the circles are touching!
	if (distance <= c1r + c2r)
	{
		return true;
	}
	return false;
}

void Collision::DrawRectangleCollision(Collision rect, Color & color)
{
	float scaleX = rect.colRadius.xScale;
	float scaleY = rect.colRadius.yScale;

	float xMin = rect.pos.x + (rect.colRadius.xScale * rect.offset.offset[0].x);
	float xMax = rect.pos.x + (rect.colRadius.xScale * rect.offset.offset[3].x);

	float yMin = rect.pos.y + (rect.colRadius.yScale * rect.offset.offset[0].y);
	float yMax = rect.pos.y + (rect.colRadius.yScale * rect.offset.offset[1].y);
	Sprite::DrawSquareLine(xMin, yMin, xMax, yMax, scaleX, scaleY, color);
}

void Collision::DrawCircleCollision(Collision circle, Color & color)
{
	float cx = circle.pos.x;
	float cy = circle.pos.y;
	float cr = circle.colRadius.xScale;
	Sprite::DrawCircleLine(cx, cy, cr, cr, 300, color);
}

bool Collision::CheckCollision(Collision * col1, Collision * col2)
{
	if (col1->colShape == CollisionShape::SQUARE && col2->colShape == CollisionShape::SQUARE)
	{
		return CheckRectangleCollision(col1, col2);
	}
	else if (col1->colShape == CollisionShape::CIRCLE && col2->colShape == CollisionShape::SQUARE) // problematic
	{
		return CheckRectangleCircleCollision(col2, col1);
	}
	else if (col1->colShape == CollisionShape::SQUARE && col2->colShape == CollisionShape::CIRCLE)
	{
		return CheckRectangleCircleCollision(col1, col2);
	}
	else if (col1->colShape == CollisionShape::CIRCLE && col2->colShape == CollisionShape::CIRCLE)
	{
		return CheckCircleCollision(col1, col2);
	}
	return false;
}

void Collision::DrawCollision(Collision col, Color & color)
{
	if (col.colShape == CollisionShape::SQUARE)
	{
		DrawRectangleCollision(col, color);
	}
	else if (col.colShape == CollisionShape::CIRCLE)
	{
		DrawCircleCollision(col, color);
	}
}
