#pragma once
#include "AllHeader.h"
#include "Vector3.h"
#include "Color.h"
#include "GameObject.h"
#include "CollideGameObject.h"
#include "PlayerData.h"
#include "GLFW/glfw3.h"

const float networkFPS = 30.0f;
const float gNetworkFrameTime = 1.0f / networkFPS;

class NeonShooter
{
protected:
	bool m_getNetUpdate = false;
	GLFWwindow* window;
	double xpos, ypos;
	double m_prevReceivedTime;

	Sprite gridSprite = Sprite("../Media/Grid1.png");
	GameObject grid = GameObject(&gridSprite);

	Sprite wallSprite = Sprite();
	Sprite obstaclesprite = Sprite("../Media/Body.png");
	Sprite bulletSprite = Sprite("../Media/Bullet.png");
	Sprite missileSprite = Sprite("../Media/Missile.png");
	Sprite playerSprite = Sprite("../Media/Player.png");
	
	std::list<BulletObject*> playerBulletList;
	std::list<MissileObject*> playerMissileList;
	std::list<BulletObject*> tempPlayerBulletList;
	std::list<BulletObject*> enemyBulletList;
	std::list<MissileObject*> enemyMissileList;

	Wall wallTemplate = Wall(&wallSprite);
	Wall wall[4];
	CircularWall obstacleTemplate = CircularWall(&obstaclesprite);
	CircularWall obstacles[4];

private:
	bool m_gameStarted;
	float playerObjectSpeedLimit = 250.0f;
	float bulletSpeed = 300.0f;
	float missileSpeed = 100.0f;
	
	float bulletFireRate = 0.5f;
	float defaultBulletFireRate = 0.5f;

	
	float missileFireRate = 0.0f;
	float defaultMissileFireRate = 10.0f;

	void LimitVelAndPos(GameObject* go);
	
	void CheckPlayerEnemyHealth(double elapsedTime);

	void CheckPlayerCollision(double elapsedTime);
	void CheckEnemyCollision(double elapsedTime);

	void CheckPlayerBulletCollision(double elapsedTime);
	void CheckEnemyBulletCollision(double elapsedTime);
	
	void UpdatePlayerBullet(double elapsedTime);
	void UpdateEnemyBullet(double elapsedTime);
	
	void DrawPlayerBullet();
	void DrawEnemyBullet();
	
	void CreateProjectile(unsigned int id, Vector3 pos, Vector3 vel, Vector3 rot, Color color, PlayerType playerType, GameObjectTag tag);
	void networkUpdate();
	void DestroyGameObjects();

public:
	GameData gameData;
	PlayerObject playerObject = PlayerObject(&playerSprite);
	PlayerObject enemyObject = PlayerObject(&playerSprite);

	NeonShooter();
	NeonShooter(GLFWwindow* win);
	~NeonShooter();

	void sendMyData(void);
	void OnReceivedOpponentData(float* data);
	void OnReceivedBulletData(unsigned int id, unsigned int tagID, float* data);

	void Keys(GLFWwindow* window);
	void MouseButtons(GLFWwindow* window, double elapsedTime);
	void MousePosition(GLFWwindow* window);

	
	float& GetMissileTimer();
	float& GetMissileTimerLimit();
	void SetWindow(GLFWwindow* win);
	void Start();
	void Restart();
	void InitializeGameWorld();
	void ExitGameWorld();
	void InitializeTutorial();
	void InitializePlayerEnemyData();
	void Update(double elapsedTime);
	void Draw();
};
