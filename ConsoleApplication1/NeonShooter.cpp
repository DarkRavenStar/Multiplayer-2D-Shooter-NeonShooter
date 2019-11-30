#include "NeonShooter.h"

NeonShooter::NeonShooter()
{
}

NeonShooter::NeonShooter(GLFWwindow * win)
{
	window = win;
}

NeonShooter::~NeonShooter()
{
	
}

void NeonShooter::networkUpdate()
{
	static double prevTime = glfwGetTime();

	double time = glfwGetTime();
	if (time - prevTime >= gNetworkFrameTime)
	{
		CheckPlayerEnemyHealth(0.0f);
		sendMyData();
		prevTime = time;
	}
}

void NeonShooter::sendMyData(void)
{
	Vector3 pos = playerObject.GetPos();
	Vector3 vel = playerObject.GetVel();
	Vector3 acc = playerObject.GetAccel();
	Vector3 rotVec = playerObject.rotVector;
	MyPhoton::getInstance().sendPlayerData(pos.x, pos.y, vel.x, vel.y, acc.x, acc.y, rotVec.x, rotVec.y);

	std::list<BulletObject*>::iterator boIter = tempPlayerBulletList.begin();
	while (boIter != tempPlayerBulletList.end())
	{
		BulletObject* po = *boIter;
		unsigned int id = po->m_ID;
		pos = po->m_Bullet.GetPos();
		vel = po->m_Bullet.GetVel();
		acc = po->m_Bullet.GetAccel();
		rotVec = po->m_Bullet.rotVector;
		MyPhoton::getInstance().sendBulletData(id, (int)po->m_Bullet.m_gameObjectTag, pos.x, pos.y, vel.x, vel.y, acc.x, acc.y, rotVec.x, rotVec.y);
		boIter++;
		//std::cout << "sendMyData: " << "tempPlayerBulletList" << std::endl;
	}
	tempPlayerBulletList.clear();
	
	std::list<MissileObject*>::iterator moIter = playerMissileList.begin();
	while (moIter != playerMissileList.end())
	{
		MissileObject* mo = *moIter;
		unsigned int id = mo->m_ID;
		pos = mo->m_Bullet.GetPos();
		vel = mo->m_Bullet.GetVel();
		acc = mo->m_Bullet.GetAccel();
		rotVec = mo->m_Bullet.rotVector;
		MyPhoton::getInstance().sendBulletData(id, (int)mo->m_Bullet.m_gameObjectTag, pos.x, pos.y, vel.x, vel.y, acc.x, acc.y, rotVec.x, rotVec.y);
		moIter++;
	}
}

void NeonShooter::OnReceivedOpponentData(float* data)
{
	if (gameData.canPlay == true && m_gameStarted == false)
	{
		m_gameStarted = true;
		enemyObject.SetPos(Vector3(data[0], data[1], 0.0f));
		enemyObject.SetRotation(data[6]);
		enemyObject.m_lastReceivedPos = Vector3(data[0], data[1], 0.0f);
		m_prevReceivedTime = glfwGetTime();
		return;
	}
	m_getNetUpdate = true;
	enemyObject.m_lastReceivedPos = Vector3(data[0], data[1], 0);
	enemyObject.SetVel(Vector3(data[2], data[3], 0));
	enemyObject.SetAccel(Vector3(data[4], data[5], 0));
	enemyObject.m_lastRotVector.x = data[6];
	enemyObject.m_lastRotVector.y = data[7];
}

void NeonShooter::OnReceivedBulletData(unsigned int id, unsigned int tagID, float* data)
{
	GameObjectTag tag = (GameObjectTag)tagID;
	if (tag == GameObjectTag::BULLET)
	{
		bool gotID = false;
		
		std::list<BulletObject*>::iterator boIter = enemyBulletList.begin();
		while (boIter != enemyBulletList.end())
		{
			BulletObject* po = *boIter;
			unsigned int poID = po->m_ID;
			if (poID == id)
			{
				gotID = true;
			}
			boIter++;
		}
		if (gotID == false)
		{	
			CreateProjectile(id, Vector3(data[0], data[1], 0.0f), Vector3(data[2], data[3], 0.0f), Vector3(data[4], data[5], 0.0f), enemyObject.GetColor(), enemyObject.playerData.playerType, tag);
		}
	}
	else if (tag == GameObjectTag::MISSILE)
	{
		bool gotID = false;

		
		std::list<MissileObject*>::iterator moIter = enemyMissileList.begin();
		while (moIter != enemyMissileList.end())
		{
			MissileObject* po = *moIter;
			unsigned int poID = po->m_ID;
			if (poID == id)
			{
				gotID = true;
				if (gotID == true)
				{
					po->m_Bullet.m_lastReceivedPos = Vector3(data[0], data[1], 0);
					po->m_Bullet.SetVel(Vector3(data[2], data[3], 0));
					po->m_Bullet.SetAccel(Vector3(data[4], data[5], 0));
					po->m_Bullet.m_lastRotVector = Vector3(data[2], data[3], 0);
					po->m_Bullet.targetPosition = Vector3(data[2], data[3], 0);
					po->m_Bullet.m_lastKnownPos = po->m_Bullet.GetPos();
				}
			}
			moIter++;
		}
		

		if (gotID == false)
		{
			CreateProjectile(id, Vector3(data[0], data[1], 0.0f), Vector3(data[2], data[3], 0.0f), Vector3(data[4], data[5], 0.0f), enemyObject.GetColor(), enemyObject.playerData.playerType, tag);
		}
	}
}

void NeonShooter::Keys(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		playerObject.SetAccel(playerObject.GetAccel() + Vector3(0.0f, -500.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		playerObject.SetAccel(playerObject.GetAccel() + Vector3(-500.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		playerObject.SetAccel(playerObject.GetAccel() + Vector3(0.0f, 500.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		playerObject.SetAccel(playerObject.GetAccel() + Vector3(500.0f, 0.0f, 0.0f));
	}
}

void NeonShooter::MouseButtons(GLFWwindow* window, double elapsedTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		static unsigned int id = 0;
		bulletFireRate -= elapsedTime;
		if (bulletFireRate <= 0.0f)
		{
			bulletFireRate = defaultBulletFireRate;
			for (int i = 0; i < 2; i++)
			{
				float playerX = playerObject.GetPos().x;
				float playerY = playerObject.GetPos().y;

				float bulletX = playerX + (i * 20.0f) - 10.0f;
				float bulletY = playerY + 40.0f;

				float playerRotAngle = playerObject.GetRotation();
				float bulletPosX = cos(playerRotAngle) * (bulletX - playerX) - sin(playerRotAngle) * (bulletY - playerY) + playerX;
				float bulletPosY = sin(playerRotAngle) * (bulletX - playerX) + cos(playerRotAngle) * (bulletY - playerY) + playerY;

				float bulletVelX = playerObject.rotVector.x * bulletSpeed;
				float bulletVelY = playerObject.rotVector.y * bulletSpeed;

				Vector3 pos = Vector3(bulletPosX, bulletPosY, 0.0f);
				Vector3 vel = Vector3(bulletVelX, bulletVelY, 0.0f);
				Vector3 accel = Vector3(0.0f, 0.0f, 0.0f);
				Color color = playerObject.GetDefaultColor();
				CreateProjectile(id, pos, vel, accel, color, playerObject.playerData.playerType, GameObjectTag::BULLET);

				id++;
				if (id > 255)
				{
					id = 0;
				}
			}
		}
	}
	
	if (missileFireRate > 0.0f)
	{
		missileFireRate -= elapsedTime;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		static unsigned int id = 0;
		if (missileFireRate <= 0.0f)
		{
			missileFireRate = defaultMissileFireRate;
			float playerX = playerObject.GetPos().x;
			float playerY = playerObject.GetPos().y;

			float bulletX = playerX;
			float bulletY = playerY + 40.0f;

			float playerRotAngle = playerObject.GetRotation();
			float bulletPosX = cos(playerRotAngle) * (bulletX - playerX) - sin(playerRotAngle) * (bulletY - playerY) + playerX;
			float bulletPosY = sin(playerRotAngle) * (bulletX - playerX) + cos(playerRotAngle) * (bulletY - playerY) + playerY;

			float bulletVelX = playerObject.rotVector.x * missileSpeed;
			float bulletVelY = playerObject.rotVector.y * missileSpeed;

			Vector3 pos = Vector3(bulletPosX, bulletPosY, 0.0f);
			Vector3 vel = Vector3(bulletVelX, bulletVelY, 0.0f);
			Vector3 accel = Vector3(0.0f, 0.0f, 0.0f);
			Color color = playerObject.GetDefaultColor();
			CreateProjectile(id, pos, vel, accel, color, playerObject.playerData.playerType, GameObjectTag::MISSILE);
			id++;
			if (id > 255)
			{
				id = 0;
			}
		}
	}
}

void NeonShooter::MousePosition(GLFWwindow * window)
{
	glfwGetCursorPos(window, &xpos, &ypos);
}

void NeonShooter::LimitVelAndPos(GameObject * go)
{
	if (go->GetVel().GetMagnitude() > playerObjectSpeedLimit)
	{
		Vector3 vec = go->GetVel();
		vec.Normalize();
		vec *= playerObjectSpeedLimit;
		go->SetVel(vec);
	}
}

void NeonShooter::CheckPlayerEnemyHealth(double elapsedTime)
{
	if (playerObject.playerData.health <= 0.0f)
	{
		if (enemyObject.playerData.health <= 0.0f)
		{
			gameData.matchResult = MatchResult::DRAW;
		}
		else if (enemyObject.playerData.health > 0)
		{
			gameData.matchResult = MatchResult::WIN;
			gameData.winner = enemyObject.playerData.playerType;
			enemyObject.playerData.score += 1;
		}
		gameData.canPlay = false;
	}
	else if (enemyObject.playerData.health <= 0.0f)
	{
		std::cout << "(enemyObject.playerData.health <= 0.0f)" << std::endl;
		if (playerObject.playerData.health <= 0.0f)
		{
			std::cout << "gameData.matchResult = MatchResult::DRAW;" << std::endl;
			gameData.matchResult = MatchResult::DRAW;
		}
		else if (playerObject.playerData.health > 0)
		{
			std::cout << "gameData.matchResult = MatchResult::WIN;" << std::endl;
			gameData.matchResult = MatchResult::WIN;
			gameData.winner = playerObject.playerData.playerType;
			playerObject.playerData.score += 1;
		}
		gameData.canPlay = false;
	}
}

void NeonShooter::CheckPlayerCollision(double elapsedTime)
{
	for (int i = 0; i < 4; i++)
	{
		playerObject.CheckCollision(&wall[i], elapsedTime);
		playerObject.CheckCollision(&obstacles[i], elapsedTime);
	}
	playerObject.CheckCollision(&enemyObject, elapsedTime);
}

void NeonShooter::CheckEnemyCollision(double elapsedTime)
{
	for (int i = 0; i < 4; i++)
	{
		enemyObject.CheckCollision(&wall[i], elapsedTime);
	}
	enemyObject.CheckCollision(&playerObject, elapsedTime);
}

void NeonShooter::CheckPlayerBulletCollision(double elapsedTime)
{	
	std::list<BulletObject*>::iterator boIter = playerBulletList.begin();
	while (boIter != playerBulletList.end())
	{
		BulletObject* bo = *boIter;
		for (int i = 0; i < 4; i++)
		{
			bo->m_Bullet.CheckCollision(&wall[i]);
			bo->m_Bullet.CheckCollision(&obstacles[i]);
		}

		std::list<BulletObject*>::iterator boIter1 = enemyBulletList.begin();
		while (boIter1 != enemyBulletList.end())
		{
			BulletObject* bo1 = *boIter1;
			bo->m_Bullet.CheckCollision(&bo1->m_Bullet);
			boIter1++;
		}

		std::list<MissileObject*>::iterator boIter2 = enemyMissileList.begin();
		while (boIter2 != enemyMissileList.end())
		{
			MissileObject* bo2 = *boIter2;
			bo->m_Bullet.CheckCollision(&bo2->m_Bullet);
			boIter2++;
		}

		bool hitEnemy = bo->m_Bullet.CheckCollision(&enemyObject);
		if (hitEnemy == true)
		{
			enemyObject.TakeDamage(bo->m_Bullet.bulletDamage);
		}

		boIter++;
	}

	

	std::list<MissileObject*>::iterator moIter = playerMissileList.begin();
	while (moIter != playerMissileList.end())
	{
		MissileObject* mo = *moIter;
		for (int i = 0; i < 4; i++)
		{
			mo->m_Bullet.CheckCollision(&wall[i]);
			mo->m_Bullet.CheckCollision(&obstacles[i]);
		}

		std::list<BulletObject*>::iterator boIter1 = enemyBulletList.begin();
		while (boIter1 != enemyBulletList.end())
		{
			BulletObject* bo1 = *boIter1;
			mo->m_Bullet.CheckCollision(&bo1->m_Bullet);
			boIter1++;
		}

		std::list<MissileObject*>::iterator boIter2 = enemyMissileList.begin();
		while (boIter2 != enemyMissileList.end())
		{
			MissileObject* bo2 = *boIter2;
			mo->m_Bullet.CheckCollision(&bo2->m_Bullet);
			boIter2++;
		}

		bool hitEnemy = mo->m_Bullet.CheckCollision(&enemyObject);
		if (hitEnemy == true)
		{
			enemyObject.TakeDamage(mo->m_Bullet.bulletDamage);
		}

		moIter++;
	}
}

void NeonShooter::CheckEnemyBulletCollision(double elapsedTime)
{
	std::list<BulletObject*>::iterator boIter = enemyBulletList.begin();
	while (boIter != enemyBulletList.end())
	{
		BulletObject* bo = *boIter;
		for (int i = 0; i < 4; i++)
		{
			bo->m_Bullet.CheckCollision(&wall[i]);
			bo->m_Bullet.CheckCollision(&obstacles[i]);
		}

		std::list<BulletObject*>::iterator boIter1 = playerBulletList.begin();
		while (boIter1 != playerBulletList.end())
		{
			BulletObject* bo1 = *boIter1;
			bo->m_Bullet.CheckCollision(&bo1->m_Bullet);
			boIter1++;
		}

		std::list<MissileObject*>::iterator boIter2 = playerMissileList.begin();
		while (boIter2 != playerMissileList.end())
		{
			MissileObject* bo2 = *boIter2;
			bo->m_Bullet.CheckCollision(&bo2->m_Bullet);
			boIter2++;
		}

		bool hitEnemy = bo->m_Bullet.CheckCollision(&playerObject);
		if (hitEnemy == true)
		{
			playerObject.TakeDamage(bo->m_Bullet.bulletDamage);
		}

		boIter++;
		//std::cout << "CheckEnemyBulletCollision: " << "enemyBulletList" << std::endl;
	}

	std::list<MissileObject*>::iterator moIter = enemyMissileList.begin();
	while (moIter != enemyMissileList.end())
	{
		MissileObject* mo = *moIter;
		for (int i = 0; i < 4; i++)
		{
			mo->m_Bullet.CheckCollision(&wall[i]);
			mo->m_Bullet.CheckCollision(&obstacles[i]);
		}

		std::list<BulletObject*>::iterator boIter1 = playerBulletList.begin();
		while (boIter1 != playerBulletList.end())
		{
			BulletObject* bo1 = *boIter1;
			mo->m_Bullet.CheckCollision(&bo1->m_Bullet);
			boIter1++;
		}

		std::list<MissileObject*>::iterator boIter2 = playerMissileList.begin();
		while (boIter2 != playerMissileList.end())
		{
			MissileObject* bo2 = *boIter2;
			mo->m_Bullet.CheckCollision(&bo2->m_Bullet);
			boIter2++;
		}

		bool hitEnemy = mo->m_Bullet.CheckCollision(&playerObject);
		if (hitEnemy == true)
		{
			playerObject.TakeDamage(mo->m_Bullet.bulletDamage);
		}

		moIter++;
		//std::cout << "CheckEnemyBulletCollision: " << "enemyMissileList" << std::endl;
	}
}

void NeonShooter::UpdatePlayerBullet(double elapsedTime)
{
	std::list<BulletObject*>::iterator boIter = playerBulletList.begin();
	while (boIter != playerBulletList.end())
	{
		float bulletHealth = (*boIter)->m_Bullet.bulletHealth;
		int id = (*boIter)->m_ID;
		if (bulletHealth <= 0.0f)
		{
			boIter = playerBulletList.erase(boIter);
		}
		else
		{
			boIter++;
		}
		//std::cout << "UpdatePlayerBullet: " << "playerBulletList - Check" << std::endl;
	}

	std::list<MissileObject*>::iterator moIter = playerMissileList.begin();
	while (moIter != playerMissileList.end())
	{
		float bulletHealth = (*moIter)->m_Bullet.bulletHealth;
		int id = (*moIter)->m_ID;
		if (bulletHealth <= 0.0f)
		{
			moIter = playerMissileList.erase(moIter);
		}
		else
		{
			moIter++;
		}
		//std::cout << "UpdatePlayerBullet: " << "playerMissileList - Check" << std::endl;
	}

	boIter = playerBulletList.begin();
	while (boIter != playerBulletList.end())
	{
		BulletObject* po = *boIter;
		po->m_Bullet.Update(elapsedTime);
		boIter++;
		//std::cout << "UpdatePlayerBullet: " << "playerBulletList - Update" << std::endl;
	}

	moIter = playerMissileList.begin();
	while (moIter != playerMissileList.end())
	{
		MissileObject* mo = *moIter;
		mo->m_Bullet.targetPosition = enemyObject.GetPos();
		mo->m_Bullet.Update(elapsedTime);
		moIter++;
		//std::cout << "UpdatePlayerBullet: " << "playerMissileList - Update" << std::endl;
	}
}

void NeonShooter::UpdateEnemyBullet(double elapsedTime)
{
	std::list<BulletObject*>::iterator boIter = enemyBulletList.begin();
	while (boIter != enemyBulletList.end())
	{
		float bulletHealth = (*boIter)->m_Bullet.bulletHealth;
		int id = (*boIter)->m_ID;
		if (bulletHealth <= 0.0f)
		{
			boIter = enemyBulletList.erase(boIter);
		}
		else
		{
			boIter++;
		}
	}

	std::list<MissileObject*>::iterator moIter = enemyMissileList.begin();
	while (moIter != enemyMissileList.end())
	{
		float bulletHealth = (*moIter)->m_Bullet.bulletHealth;
		int id = (*moIter)->m_ID;
		if (bulletHealth <= 0.0f)
		{
			moIter = enemyMissileList.erase(moIter);
		}
		else
		{
			moIter++;
		}
		//std::cout << "UpdatePlayerBullet: " << "enemyMissileList - Check" << std::endl;
	}

	boIter = enemyBulletList.begin();
	while (boIter != enemyBulletList.end())
	{
		BulletObject* po = *boIter;
		po->m_Bullet.Update(elapsedTime);
		boIter++;
		//std::cout << "UpdatePlayerBullet: " << "enemyBulletList - Update" << std::endl;
	}

	moIter = enemyMissileList.begin();
	while (moIter != enemyMissileList.end())
	{
		MissileObject* po = *moIter;
		po->m_Bullet.targetPosition = playerObject.GetPos();
		po->m_Bullet.Update(elapsedTime);
		moIter++;
		//std::cout << "UpdatePlayerBullet: " << "enemyMissileList - Update" << std::endl;
	}
}

void NeonShooter::DrawPlayerBullet()
{
	std::list<BulletObject*>::iterator poIter = playerBulletList.begin();
	while (poIter != playerBulletList.end())
	{
		BulletObject* po = *poIter;
		po->m_Bullet.Draw();
		poIter++;
		//std::cout << "DrawPlayerBullet: " << "playerBulletList" << std::endl;
	}

	

	std::list<MissileObject*>::iterator moIter = playerMissileList.begin();
	while (moIter != playerMissileList.end())
	{
		MissileObject* po = *moIter;
		po->m_Bullet.Draw();
		moIter++;
		//std::cout << "DrawPlayerBullet: " << "playerMissileList" << std::endl;
	}
}

void NeonShooter::DrawEnemyBullet()
{
	std::list<BulletObject*>::iterator poIter = enemyBulletList.begin();
	while (poIter != enemyBulletList.end())
	{
		BulletObject* po = *poIter;
		po->m_Bullet.Draw();
		poIter++;
		//std::cout << "DrawEnemyBullet: " << "enemyBulletList" << std::endl;
	}


	std::list<MissileObject*>::iterator moIter = enemyMissileList.begin();
	while (moIter != enemyMissileList.end())
	{
		MissileObject* po = *moIter;
		po->m_Bullet.Draw();
		moIter++;
		//std::cout << "DrawEnemyBullet: " << "enemyMissileList" << std::endl;
	}
}

void NeonShooter::CreateProjectile(unsigned int id, Vector3 pos, Vector3 vel, Vector3 accel, Color color, PlayerType playerType, GameObjectTag tag)
{
	if (playerType == playerObject.playerData.playerType)
	{
		if (tag == GameObjectTag::BULLET)
		{
			Bullet projectile = Bullet(&bulletSprite);
			projectile.SetPos(pos);
			projectile.SetVel(vel);
			projectile.SetAccel(accel);
			projectile.SetColor(color);
			projectile.rotVector = vel;
			projectile.UpdateRotation();
			projectile.SetPosOffset(MatrixOffset::Center());
			projectile.SetScale(5, 5);
			projectile.SetCollisionData();
			projectile.speed = bulletSpeed;
			playerBulletList.push_back(new BulletObject(id, projectile));
			tempPlayerBulletList.push_back(playerBulletList.back());
		}
		else if (tag == GameObjectTag::MISSILE)
		{
			Missile projectile = Missile(&missileSprite);
			projectile.SetPos(pos);
			projectile.SetVel(vel);
			projectile.SetAccel(accel);
			projectile.SetColor(color);
			projectile.rotVector = vel;
			projectile.UpdateRotation();
			projectile.SetPosOffset(MatrixOffset::Center());
			projectile.SetScale(20, 20);
			projectile.SetCollisionData();
			projectile.speed = missileSpeed;
			playerMissileList.push_back(new MissileObject(id, projectile));
		}
	}
	else if(playerType == enemyObject.playerData.playerType)
	{
		if (tag == GameObjectTag::BULLET)
		{
			Bullet projectile = Bullet(&bulletSprite);
			projectile.SetPos(pos);
			projectile.SetVel(vel);
			projectile.SetAccel(accel);
			projectile.SetColor(color);
			projectile.rotVector = vel;
			projectile.UpdateRotation();
			projectile.SetPosOffset(MatrixOffset::Center());
			projectile.SetScale(5, 5);
			projectile.SetCollisionData();
			projectile.speed = bulletSpeed;
			enemyBulletList.push_back(new BulletObject(id, projectile));
		}
		else if (tag == GameObjectTag::MISSILE)
		{
			Missile projectile = Missile(&missileSprite);
			projectile.SetPos(pos);
			projectile.SetVel(vel);
			projectile.SetAccel(accel);
			projectile.SetColor(color);
			projectile.rotVector = vel;
			projectile.UpdateRotation();
			projectile.SetPosOffset(MatrixOffset::Center());
			projectile.SetScale(20, 20);
			projectile.SetCollisionData();
			projectile.speed = missileSpeed;
			enemyMissileList.push_back(new MissileObject(id, projectile));
		}
	}
}

float & NeonShooter::GetMissileTimer()
{
	return missileFireRate;
}

float & NeonShooter::GetMissileTimerLimit()
{
	return defaultMissileFireRate;
}

void NeonShooter::SetWindow(GLFWwindow * win)
{
	window = win;
}

void NeonShooter::Start()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	m_gameStarted = false;

	Color tempRedColor = Color(Color::Red.r, Color::Red.g, Color::Red.b, 0.3f);
	Color tempWhiteColor = Color(Color::White.r, Color::White.g, Color::White.b, 0.3f);
	wallTemplate.SetColor(tempRedColor);
	wallTemplate.SetPosOffset(MatrixOffset::CornerUpperLeft());
	wallTemplate.SetScale(17, 17 * 30.1);
	wallTemplate.SetPos(Vector3(0.0f, 0.0f, 0.0f));
	wallTemplate.SetCollisionData();

	obstacleTemplate.SetPosOffset(MatrixOffset::Center());
	obstacleTemplate.SetColor(tempWhiteColor);
	obstacleTemplate.SetScale(17, 17 * 30.1);
	obstacleTemplate.SetPos(Vector3(0.0f, 0.0f, 0.0f));
	obstacleTemplate.SetCollisionData();

	for (int i = 0; i < 4; i++)
	{
		wall[i] = wallTemplate;
		obstacles[i] = obstacleTemplate;
	}

	wall[1].SetScale(17 * 40.1, 17);
	wall[1].SetCollisionData();
	wall[1].UpdateCollisionPosition();

	wall[2].SetScale(17, 17 * 30.1);
	wall[2].SetPos(Vector3(17 * 78.3f, 0.0f, 0.0f));
	wall[2].SetCollisionData();
	wall[2].UpdateCollisionPosition();

	wall[3].SetScale(17 * 40.1, 17);
	wall[3].SetPos(Vector3(0.0f, 17 * 58.3, 0.0f));
	wall[3].SetCollisionData();
	wall[3].UpdateCollisionPosition();

	obstacles[0].SetScale(50.0f, 50.0f);
	obstacles[0].SetPos(Vector3(gridSprite.m_width * 0.5f, gridSprite.m_height * 0.5f, 0.0f));
	obstacles[0].SetCollisionData();
	obstacles[0].UpdateCollisionPosition();

	obstacles[1].SetScale(50.0f, 50.0f);
	obstacles[1].SetPos(Vector3(gridSprite.m_width * 0.5f, gridSprite.m_height * 1.5f, 0.0f));
	obstacles[1].SetCollisionData();
	obstacles[1].UpdateCollisionPosition();

	obstacles[2].SetScale(50.0f, 50.0f);
	obstacles[2].SetPos(Vector3(gridSprite.m_width * 1.5f, gridSprite.m_height * 1.5f, 0.0f));
	obstacles[2].SetCollisionData();
	obstacles[2].UpdateCollisionPosition();

	obstacles[3].SetScale(50.0f, 50.0f);
	obstacles[3].SetPos(Vector3(gridSprite.m_width * 1.5f, gridSprite.m_height * 0.5f, 0.0f));
	obstacles[3].SetCollisionData();
	obstacles[3].UpdateCollisionPosition();

	playerObject.SetPosOffset(MatrixOffset::Player());
	playerObject.SetScale(26.8f * 1, 38.4f * 1);
	playerObject.SetPos(Vector3(-9000.0f, -6000.0f, 0.0f));

	enemyObject.SetPosOffset(MatrixOffset::Player());
	enemyObject.SetScale(26.8f * 1, 38.4f * 1);
	enemyObject.SetPos(Vector3(9000.0f, 6000.0f, 0.0f));

	grid.SetColor(Color::White);
	grid.SetRotation(0.0f);
	grid.SetScale(gridSprite.m_width / 2, gridSprite.m_height / 2);
	grid.SetPosOffset(MatrixOffset::CornerUpperLeft());
}

void NeonShooter::Restart()
{
	DestroyGameObjects();
	playerObject.Restart();
	enemyObject.Restart();
	InitializePlayerEnemyData();
	gameData.matchResult = MatchResult::NONE;
	gameData.winner = PlayerType::NO_PLAYER;
	gameData.canPlay = true;
}

void NeonShooter::InitializeGameWorld()
{
	MyPhoton::getInstance().connect();
}

void NeonShooter::ExitGameWorld()
{
	DestroyGameObjects();
	if (gameData.gameState == GameState::GAME_WORLD)
	{
		MyPhoton::getInstance().disconnect();
	}
	gameData.matchResult = MatchResult::NONE;
	gameData.winner = PlayerType::NO_PLAYER;
	gameData.canPlay = false;
	gameData.gameState = GameState::HOME;
}

void NeonShooter::InitializeTutorial()
{
	playerObject.playerData.playerType = PlayerType::PLAYER_1;
	enemyObject.playerData.playerType = PlayerType::PLAYER_2;
	InitializePlayerEnemyData();
	gameData.gameState = GameState::TUTORIAL;
	gameData.canPlay = true;
}

void NeonShooter::DestroyGameObjects()
{
	while (!playerBulletList.empty())
	{
		delete playerBulletList.back();
		playerBulletList.pop_back();
	}
	playerBulletList.clear();

	while (!playerMissileList.empty())
	{
		delete playerMissileList.back();
		playerMissileList.pop_back();
	}
	playerMissileList.clear();

	while (!enemyBulletList.empty())
	{
		delete enemyBulletList.back();
		enemyBulletList.pop_back();
	}
	enemyBulletList.clear();

	while (!enemyMissileList.empty())
	{
		delete enemyMissileList.back();
		enemyMissileList.pop_back();
	}
	enemyMissileList.clear();
}

void NeonShooter::InitializePlayerEnemyData()
{
	playerObject.InitializePlayerObject();
	enemyObject.InitializePlayerObject();
}

void NeonShooter::Update(double elapsedTime)
{
	MyPhoton::getInstance().run();
	if ((gameData.gameState == GameState::TUTORIAL || gameData.gameState == GameState::GAME_WORLD) && gameData.canPlay == true)
	{
		Keys(window);
		MouseButtons(window, elapsedTime);
		MousePosition(window);

		CheckPlayerBulletCollision(elapsedTime);
		CheckEnemyBulletCollision(elapsedTime);

		CheckPlayerCollision(elapsedTime);
		CheckEnemyCollision(elapsedTime);

		UpdatePlayerBullet(elapsedTime);
		UpdateEnemyBullet(elapsedTime);

		playerObject.Update(elapsedTime);
		playerObject.SetAccel(Vector3(0.0f, 0.0f, 0.0f));
		LimitVelAndPos(&playerObject);

		int midWidth = 800 / 2;
		int midHeight = 600 / 2;
		float mouseOffsetX = playerObject.GetPos().x - midWidth;
		float mouseOffsetY = playerObject.GetPos().y - midHeight;

		Vector3 mouseOffset = Vector3(xpos + mouseOffsetX, ypos + mouseOffsetY, 0.0f);
		playerObject.targetPosition = mouseOffset;
		playerObject.SetRotationToTarget(elapsedTime);

		

		if (gameData.gameState == GameState::GAME_WORLD)
		{
			networkUpdate();
			if (m_gameStarted == true)
			{
				enemyObject.UpdateFromNetwork(elapsedTime);
				LimitVelAndPos(&enemyObject);
			}
		}
		else
		{
			enemyObject.Update(elapsedTime);
			LimitVelAndPos(&enemyObject);
		}

		
	}
	
}

void NeonShooter::Draw()
{
	if (gameData.gameState == GameState::TUTORIAL || gameData.gameState == GameState::GAME_WORLD)
	{
		grid.SetPos(Vector3(0.0f, 0.0f, 0.0f));
		grid.Draw();
		grid.SetPos(Vector3(gridSprite.m_width, 0.0f, 0.0f));
		grid.Draw();
		grid.SetPos(Vector3(0.0f, gridSprite.m_height, 0.0f));
		grid.Draw();
		grid.SetPos(Vector3(gridSprite.m_width, gridSprite.m_height, 0.0f));
		grid.Draw();

		for (int i = 0; i < 4; i++)
		{
			wall[i].Draw();
			wall[i].DrawDebug(Color::Red);
			obstacles[i].Draw();
			obstacles[i].DrawDebug(Color::White);
		}

		playerObject.Draw();
		DrawPlayerBullet();
		enemyObject.Draw();
		DrawEnemyBullet();
	}
}

