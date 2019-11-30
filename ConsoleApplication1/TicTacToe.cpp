#include "TicTacToe.h"

TicTacToe::TicTacToe(GLFWwindow* win)
{
	window = win;
}

void TicTacToe::Start()
{
	grid = Sprite("../Media/Grid.png");
	oWin = Sprite("../Media/O Win.png");
	xWin = Sprite("../Media/X Win.png");
	xTile = Sprite("../Media/X.png");
	oTile = Sprite("../Media/O.png");

	gridObject = GameObject(&grid);
	gridObject.SetColor(Color::White);
	gridObject.SetRotation(0.0f);
	gridObject.SetScale(150.0f, 150.0f);

	bgObject = GameObject();
	bgObject.SetColor(Color::Black);
	bgObject.SetRotation(0.0f);
	bgObject.SetScale(200.0f, 200.0f);

	Reset();
}

void TicTacToe::Update()
{
	if (gameData->doReset == true)
	{
		Reset();
		gameData->matchResult = MatchResult::NONE;
		gameData->winner = PlayerType::NO_PLAYER;
		gameData->canPlay = true;
		gameData->doReset = false;
	}

	if (transferData->execute == true)
	{
		UIntPacker::getInstance().SetData(transferData->playerData);
		unsigned int i = UIntPacker::getInstance().extract(1);
		if (i == 1)
		{
			transferData->execute = false;
			gameData->doReset = true;
		}
	}

	if(gameData->canPlay == true)
	{
		if (gameData->matchResult == MatchResult::NONE)
		{
			if (gameData->curPlayer == playerData->playerType)
			{
				Input();
			}
			else if (gameData->curPlayer == enemyData->playerType)
			{
				ServerInput();
			}
		}
	}

	UpdateObjectPosition();
}

void TicTacToe::Draw()
{
	gridObject.Draw();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Tile* temp = &tiles[i][j];
			temp->Draw();
		}
	}
	
}

void TicTacToe::Reset()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Tile* temp = &tiles[i][j];
			temp->tileState = TileState::NONE;
			temp->SetSprite(NULL);
			temp->SetColor(Color::White);
			temp->SetRotation(0.0f);
			temp->SetScale(48.0f, 48.0f);
			temp->originScale = temp->GetScale();
			temp->curScale = temp->originScale;
			temp->doHighlight = false;

			gridTile[i][j] = -1;
		}
	}
}

void TicTacToe::Input()
{
	static double x, y;
	glfwGetCursorPos(window, &x, &y);
	int leftMousebutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	TileInput(leftMousebutton, x, y);
}

void TicTacToe::TileInput(int button, float xpos, float ypos)
{
	static bool IsPressed = false;
	if (button == GLFW_PRESS && IsPressed == false)
	{
		IsPressed = true;
		for (unsigned i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 3; j++)
			{
				Tile* tempTile = &tiles[i][j];
				if (tempTile->CheckBoxArea(xpos, m_height - ypos) == true && tempTile->tileState == TileState::NONE)
				{
					tempTile->tileState = TileState::SELECTED;
					std::string resetResult = gameData->doReset ? "True" : "False";
					std::cout << "Do Reset: " << resetResult << std::endl;
					UIntPacker::getInstance().pack(gameData->doReset, 1);
					UIntPacker::getInstance().pack(i, 2);
					UIntPacker::getInstance().pack(j, 2);
					MyPhoton::getInstance().sendMyData(UIntPacker::getInstance().GetData());

					if (playerData->playerType == PlayerType::PLAYER_1)
					{
						tempTile->selectedSprite = &oTile;
						tempTile->releasedSprite = &oWin;
					}
					else if (playerData->playerType == PlayerType::PLAYER_2)
					{
						tempTile->selectedSprite = &xTile;
						tempTile->releasedSprite = &xWin;
					}
					gridTile[i][j] = (int)playerData->playerType;
					MatchResult result = WinLoseCondition((int)playerData->playerType);
					if (result == MatchResult::DRAW)
					{
						gameData->matchResult = result;
						gameData->curPlayer = enemyData->playerType;
					}
					if (result == MatchResult::WIN)
					{
						gameData->matchResult = result;
						gameData->winner = playerData->playerType;
						gameData->curPlayer = playerData->playerType;
						playerData->score++;
					}
					else if (result == MatchResult::NONE)
					{
						gameData->curPlayer = enemyData->playerType;
					}
					break;
				}
			}
		}
	}

	if (button == GLFW_RELEASE && IsPressed == true)
	{
		IsPressed = false;
	}
}

void TicTacToe::ServerInput()
{
	if (transferData->execute == true)
	{
		unsigned int i = UIntPacker::getInstance().extract(2);
		unsigned int j = UIntPacker::getInstance().extract(2);
		std::cout << "Data: " << std::bitset<8>(transferData->playerData) << " - I: " << i << " | J: " << j << std::endl;
		
		Tile* tempTile = &tiles[i][j];
		gridTile[i][j] = (int)enemyData->playerType;
		
		if (tempTile->tileState == TileState::NONE)
		{
			tempTile->tileState = TileState::SELECTED;
			if (enemyData->playerType == PlayerType::PLAYER_1)
			{
				tempTile->selectedSprite = &oTile;
				tempTile->releasedSprite = &oWin;
			}
			else if (enemyData->playerType == PlayerType::PLAYER_2)
			{
				tempTile->selectedSprite = &xTile;
				tempTile->releasedSprite = &xWin;
			}
			transferData->execute = false;

			MatchResult result = WinLoseCondition((int)enemyData->playerType);
			if (result == MatchResult::DRAW)
			{
				gameData->matchResult = result;
				gameData->curPlayer = playerData->playerType;
			}
			else if (result == MatchResult::WIN)
			{
				gameData->matchResult = result;
				gameData->winner = enemyData->playerType;
				gameData->curPlayer = enemyData->playerType;
				enemyData->score++;
			}
			else if (result == MatchResult::NONE)
			{
				gameData->curPlayer = playerData->playerType;
			}
		}
	}
}

void TicTacToe::UpdateObjectPosition()
{
	glfwGetFramebufferSize(window, &m_width, &m_height);
	Vector3 center = Vector3(m_width / 2, m_height / 2, 0.0f);
	gridObject.SetPos(center);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Tile* temp = &tiles[i][j];
			Vector3 offset = Vector3(center.x + (i * 51.0f * 2) - (1 * 51.0f * 2), center.y - (j * 51.0f * 2) + (1 * 51.0f * 2), 0.0f);
			temp->SetPos(offset);
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Tile* temp = &tiles[i][j];
			temp->Update();
		}
	}
}

bool TicTacToe::CheckTileFilled()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (gridTile[i][j] == -1)
			{
				return false;
			}
		}
	}

	return true;
}

void TicTacToe::SetTransferData(TransferData* data)
{
	transferData = data;
}

void TicTacToe::SetPlayerData(PlayerData* data)
{
	playerData = data;
}

void TicTacToe::SetEnemyData(PlayerData* data)
{
	enemyData = data;
}

void TicTacToe::SetGameData(GameData* data)
{
	gameData = data;
}

MatchResult TicTacToe::WinLoseCondition(int num)
{
	drawTile.clear();
	CheckHorizontal(num);
	CheckVertical(num);
	CheckDiagonal(num);
	

	if (drawTile.empty() == false)
	{
		for (int i = 0; i < drawTile.size(); i++)
		{
			drawTile[i]->doHighlight = true;
		}
		return MatchResult::WIN;
	}
	if (drawTile.empty() == true && CheckTileFilled() == true)
	{
		return MatchResult::DRAW;
	}

	return MatchResult::NONE;
}

bool TicTacToe::TilesFinishAnimation()
{
	for (unsigned i = 0; i < 3; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			Tile* tempTile = &tiles[i][j];
			if (tempTile->life > 0.0f)
			{
				return false;
			}
		}
	}
	return true;
}

bool TicTacToe::CheckHorizontal(int num)
{
	for (unsigned i = 0; i < 3; i++)
	{
		if (gridTile[0][i] == num &&
			gridTile[1][i] == num &&
			gridTile[2][i] == num)
		{
			drawTile.push_back(&tiles[0][i]);
			drawTile.push_back(&tiles[1][i]);
			drawTile.push_back(&tiles[2][i]);
			return true;
		}
	}

	return false;
}

bool TicTacToe::CheckDiagonal(int num)
{
	if (gridTile[0][0] == num &&
		gridTile[1][1] == num &&
		gridTile[2][2] == num)
	{
		drawTile.push_back(&tiles[0][0]);
		drawTile.push_back(&tiles[1][1]);
		drawTile.push_back(&tiles[2][2]);
		return true;
	}
	else if (gridTile[2][0] == num &&
			 gridTile[1][1] == num &&
			 gridTile[0][2] == num)
	{
		drawTile.push_back(&tiles[2][0]);
		drawTile.push_back(&tiles[1][1]);
		drawTile.push_back(&tiles[0][2]);
		return true;
	}
	return false;
}

bool TicTacToe::CheckVertical(int num)
{
	for (unsigned i = 0; i < 3; i++)
	{
		if (gridTile[i][0] == num &&
			gridTile[i][1] == num &&
			gridTile[i][2] == num)
		{
			drawTile.push_back(&tiles[i][0]);
			drawTile.push_back(&tiles[i][1]);
			drawTile.push_back(&tiles[i][2]);
			return true;;
		}
	}

	return false;
}


