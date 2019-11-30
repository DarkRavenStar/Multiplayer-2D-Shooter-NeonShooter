#pragma once

#include "AllHeader.h"
#include "Sprite.h"
#include "GameObject.h"
#include "Vector3.h"
#include "GLFW/glfw3.h"
#include "Tile.h"
#include "PlayerData.h"
#include "DataPacker.h"

class TicTacToe
{
private:
	GLFWwindow* window;
	int m_width;
	int m_height;
	bool doAnimation;

	Sprite grid;
	Sprite oWin;
	Sprite xWin;
	Sprite xTile;
	Sprite oTile;
	Sprite drawLine;

	Tile tiles[3][3];
	unsigned int gridTile[3][3];

	GameObject gridObject;
	GameObject bgObject;
	GameObject drawLineObject[3];

	PlayerData* playerData;
	PlayerData* enemyData;
	GameData* gameData;

	std::vector<Tile*> drawTile;

private:
	MatchResult WinLoseCondition(int num);
	bool TilesFinishAnimation();

	bool CheckHorizontal(int num);
	bool CheckDiagonal(int num);
	bool CheckVertical(int num);
	
	void Input();
	void TileInput(int button, float xpos, float ypos);
	void ServerInput();

	void UpdateObjectPosition();
	bool CheckTileFilled();

public:
	TicTacToe(GLFWwindow* win);
	void Start();
	void Update();
	void Draw();
	void Reset();
	void SetTransferData(TransferData* data);
	void SetPlayerData(PlayerData* data);
	void SetEnemyData(PlayerData* data);
	void SetGameData(GameData* data);
};