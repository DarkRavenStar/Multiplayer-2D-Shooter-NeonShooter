#pragma once
#include "AllHeader.h"
#include <LoadBalancing-cpp/inc/Client.h>
#include "PlayerData.h"
#include "GameObjectTag.h"
#include "DataPacker.h"


struct EventCode
{
	static const nByte PLAYER_NUM = 10;
	static const nByte PLAYER_POS = 11;
	static const nByte PLAYER_STATE = 12;
	static const nByte SHOOT = 31;
	static const nByte BULLET_ID = 41;
	static const nByte BULLET_TYPE = 42;
	static const nByte BULLET_POS = 43;
	static const nByte GAME_STATE = 51;
};

class MyPhoton : private ExitGames::LoadBalancing::Listener
{
public:
	void(*OnReceivedOpponentData)(float* data);
	void(*OnReceivedBulletData)(unsigned int id, unsigned int tagID, float* data);
	void(*InitializePlayerEnemyData)();
	void(*RestartGame)();
	void(*ExitGameWorld)();
	PlayerData* playerData;
	PlayerData* enemyData;
	GameData* gameData;

public:
	MyPhoton();
	void run(void);
	void connect(void);
	void disconnect(void);

	static MyPhoton& getInstance()
	{
		static MyPhoton instance;
		return instance;
	}

	void sendMyData(unsigned char value);
	void sendTurnNum();
	void sendReset(bool restart, bool exit);
	void sendPlayerData(float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float rotX, float rotY);
	void sendBulletData(unsigned int id, unsigned int tagID, float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float rotX, float rotY);
private:
	// receive and print out debug out here
	virtual void debugReturn(int debugLevel, const ExitGames::Common::JString& string);

	// implement your error-handling here
	virtual void connectionErrorReturn(int errorCode);
	virtual void clientErrorReturn(int errorCode);
	virtual void warningReturn(int warningCode);
	virtual void serverErrorReturn(int errorCode);

	// events, triggered by certain operations of all players in the same room
	virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
	virtual void leaveRoomEventAction(int playerNr, bool isInactive);
	virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);

	virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats);
	virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);

	// callbacks for operations on PhotonLoadBalancing server
	virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster);
	virtual void disconnectReturn(void);
	virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
	virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
	virtual void joinLobbyReturn(void);
	virtual void leaveLobbyReturn(void);

	ExitGames::LoadBalancing::Client mLoadBalancingClient;
};