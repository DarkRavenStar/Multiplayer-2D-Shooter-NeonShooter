#include "Sprite.h"


Sprite::Sprite()
{
    
}

Sprite::Sprite(const std::string& file)
{
    const char* texturePath = file.c_str();
	glGenTextures(1, &m_textureID);
	LoadTexture(texturePath);
}

Sprite::~Sprite()
{

}

void Sprite::LoadTexture(const char* path)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// bilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, &m_width, &m_height, &nrChannels, STBI_rgb_alpha); //0 - Auto or 4 - RGBA
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);

}

void Sprite::Draw(float x, float y, float z, float rotation, float scaleX, float scaleY, Color& color)
{
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	DrawSquare(x, y, z, rotation, scaleX, scaleY, color);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

void Sprite::Draw(Vector3 pos, MatrixOffset offset, float rotation, float scaleX, float scaleY, Color& color)
{
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);

	DrawSquare(pos, offset, rotation, scaleX, scaleY, color);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

void Sprite::DrawSquareLine(Vector3 pos, MatrixOffset offset, float rotation, float scaleX, float scaleY, Color& color)
{
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);

	Vector3 pos1 = Vector3(xCoord(offset.offset[0].x * scaleX, offset.offset[0].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[0].x * scaleX, offset.offset[0].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos2 = Vector3(xCoord(offset.offset[1].x * scaleX, offset.offset[1].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[1].x * scaleX, offset.offset[1].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos3 = Vector3(xCoord(offset.offset[2].x * scaleX, offset.offset[2].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[2].x * scaleX, offset.offset[2].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos4 = Vector3(xCoord(offset.offset[3].x * scaleX, offset.offset[3].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[3].x * scaleX, offset.offset[3].y * scaleY, rotation, 1, pos.y), pos.z);

	glColor4f(color.r, color.g, color.b, color.a);
	
	glVertex2f(pos1.x, pos1.y);
	glVertex2f(pos2.x, pos2.y);

	glVertex2f(pos2.x, pos2.y);
	glVertex2f(pos3.x, pos3.y);

	glVertex2f(pos3.x, pos3.y);
	glVertex2f(pos4.x, pos4.y);

	glVertex2f(pos4.x, pos4.y);
	glVertex2f(pos1.x, pos1.y);
	
	glEnd();
	glPopMatrix();
}

void Sprite::DrawSquareLine(float xMin, float yMin, float xMax, float yMax, float scaleX, float scaleY, Color & color)
{
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex2f(xMin, yMin);
	glVertex2f(xMin, yMax);

	glVertex2f(xMin, yMax);
	glVertex2f(xMax, yMax);

	glVertex2f(xMax, yMax);
	glVertex2f(xMax, yMin);

	glVertex2f(xMax, yMin);
	glVertex2f(xMin, yMin);

	glEnd();
	glPopMatrix();
}

void Sprite::DrawCircleLine(float x, float y, float scaleX, float scaleY, int numberOfSides, Color & color)
{
	float r = scaleX;
	float DEF_D = 1.0f;
	float k = 0.0f;

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);

	glColor4f(color.r, color.g, color.b, color.a);
	for (k = 0; k <= 360; k += DEF_D)
	{
		//glVertex2f(x, y);
		glVertex2f(x + cos(k) * r, y + sin(k) * r);
		glVertex2f(x + cos(k + DEF_D * 0.1) * r, y + sin(k + DEF_D * 0.1) * r);
	}

	glEnd();
	glPopMatrix();
}

void Sprite::DrawSquare(Vector3 pos, MatrixOffset offset, float rotation, float scaleX, float scaleY, Color& color)
{
	float tempScaleX = scaleX;
	glColor4f(color.r, color.g, color.b, color.a);

	Vector3 pos1 = Vector3(xCoord(offset.offset[0].x * scaleX, offset.offset[0].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[0].x * scaleX, offset.offset[0].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos2 = Vector3(xCoord(offset.offset[1].x * scaleX, offset.offset[1].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[1].x * scaleX, offset.offset[1].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos3 = Vector3(xCoord(offset.offset[2].x * scaleX, offset.offset[2].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[2].x * scaleX, offset.offset[2].y * scaleY, rotation, 1, pos.y), pos.z);
	Vector3 pos4 = Vector3(xCoord(offset.offset[3].x * scaleX, offset.offset[3].y * scaleY, rotation, 1, pos.x), yCoord(offset.offset[3].x * scaleX, offset.offset[3].y * scaleY, rotation, 1, pos.y), pos.z);

	glTexCoord2f(0.0f, 0.0f);  glVertex3f(pos1.x, pos1.y, pos1.z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(pos2.x, pos2.y, pos2.z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(pos3.x, pos3.y, pos3.z);

	glTexCoord2f(1.0f, 1.0f);  glVertex3f(pos3.x, pos3.y, pos3.z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(pos4.x, pos4.y, pos4.z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(pos1.x, pos1.y, pos1.z);
	
}

void Sprite::DrawSquare(float x, float y, float z, float rotation, float scaleX, float scaleY, Color& color)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-1.0f, -1.0f, rotation, scaleX, x), yCoord(-1.0f, -1.0f, rotation, scaleY, y), z);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(xCoord(-1.0f, 1.0f, rotation, scaleX, x), yCoord(-1.0f, 1.0f, rotation, scaleY, y), z);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(1.0f, 1.0f, rotation, scaleX, x), yCoord(1.0f, 1.0f, rotation, scaleY, y), z);

	glTexCoord2f(1.0f, 1.0f);  glVertex3f(xCoord(1.0f, 1.0f, rotation, scaleX, x), yCoord(1.0f, 1.0f, rotation, scaleY, y), z);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(xCoord(1.0f, -1.0f, rotation, scaleX, x), yCoord(1.0f, -1.0f, rotation, scaleY, y), z);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(xCoord(-1.0f, -1.0f, rotation, scaleX, x), yCoord(-1.0f, -1.0f, rotation, scaleY, y), z);
}