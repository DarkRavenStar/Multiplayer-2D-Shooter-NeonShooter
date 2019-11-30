#pragma once
#include "AllHeader.h"

enum PlayerType
{
	NO_PLAYER,
	PLAYER_1,
	PLAYER_2,
};

enum MatchResult
{
	NONE,
	DRAW,
	WIN,
	LOSE
};

enum GameState
{
	HOME,
	WAITING,
	TUTORIAL,
	GAME_WORLD
};

struct PlayerData
{
	PlayerType playerType = PlayerType::NO_PLAYER;
	int health = 100;
	int maxHealth = 100;
	int score = 0;
};

struct GameData
{
	bool canPlay = false;
	bool doReset = false;
	MatchResult matchResult = MatchResult::NONE;
	PlayerType winner = PlayerType::NO_PLAYER;
	GameState gameState = GameState::HOME;
	int turnNum;
};
