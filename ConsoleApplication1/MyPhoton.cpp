#include "MyPhoton.h"
#include <iostream>

static const ExitGames::Common::JString appId = L"c84f2ee7-541c-46e8-bc4f-16c7e5aa270d"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// functions
MyPhoton::MyPhoton() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT)
{
}

void MyPhoton::connect(void)
{
	std::wcout<<"Connecting..."<<std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void MyPhoton::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// protocol implementations

void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout<<"connectionErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout<<"clientErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout<<"warningReturn : "<<warningCode<<std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout<<"serverErrorReturn : "<<errorCode<<std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << "joinRoomEventAction: "<<std::endl;

	if (playernrs.getSize() == 2)
	{
		sendTurnNum();
	}

}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout<<"leaveRoomEventAction"<<std::endl;
	gameData->canPlay = false;
	gameData->doReset = true;
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if(errorCode)
		std::wcout<<"Failed to connect : "<<errorString.cstr()<<std::endl;
	else
	{
		std::wcout<<"Connected to Photon Server."<<std::endl;
		
		std::wcout<<"Trying to join a random room..."<<std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout<<"disconnected"<<std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"createRoomReturn"<<std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinOrCreateRoomReturn"<<std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinRoomReturn" << localPlayerNr << std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout << "joinRandomRoomReturn:" << localPlayerNr << std::endl;

	if(errorCode)
	{
		std::wcout<<"Failed to join a random room : "<<errorCode<<", "<<errorString.cstr()<<std::endl;

		if(errorCode == 32760) //no match found error code
		{
			std::wcout<<"Creating a room..."<<std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(2));
		}

		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"leaveRoomReturn"<<std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout<<"joinLobbyReturn"<<std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout<<"leaveLobbyReturn"<<std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsResponse"<<std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsUpdate"<<std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}

void MyPhoton::sendMyData(unsigned char value)
{
	unsigned char data[1];
	data[0] = value;
	mLoadBalancingClient.opRaiseEvent(true, data, 1, 1);
}

void MyPhoton::sendTurnNum()
{
	unsigned char data[1];
	gameData->turnNum = rand() % 10 + 1;
	std::cout << gameData->turnNum << std::endl;
	data[0] = gameData->turnNum;
	mLoadBalancingClient.opRaiseEvent(true, data, 1, EventCode::PLAYER_NUM);
}

void MyPhoton::sendReset(bool restart, bool exit)
{
	unsigned char data[1];
	UIntPacker::getInstance().pack(restart, 1);
	UIntPacker::getInstance().pack(exit, 1);
	data[0] = UIntPacker::getInstance().GetData();
	mLoadBalancingClient.opRaiseEvent(true, data, 1, EventCode::GAME_STATE);
}

void MyPhoton::sendPlayerData(float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float rotX, float rotY)
{
	float data[8];
	data[0] = xpos;
	data[1] = ypos;
	data[2] = xvel;
	data[3] = yvel;
	data[4] = xaccel;
	data[5] = yaccel;
	data[6] = rotX;
	data[7] = rotY;

	mLoadBalancingClient.opRaiseEvent(true, data, 8, EventCode::PLAYER_POS);
}

void MyPhoton::sendBulletData(unsigned int id, unsigned int tagID, float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float rotX, float rotY)
{
	unsigned char bulletID[1];
	bulletID[0] = id;

	unsigned char bulletType[1];
	bulletType[0] = tagID;

	float data[8];
	data[0] = xpos;
	data[1] = ypos;
	data[2] = xvel;
	data[3] = yvel;
	data[4] = xaccel;
	data[5] = yaccel;
	data[6] = rotX;
	data[7] = rotY;

	mLoadBalancingClient.opRaiseEvent(true, bulletID, 1, EventCode::BULLET_ID);
	mLoadBalancingClient.opRaiseEvent(true, bulletType, 1, EventCode::BULLET_TYPE);
	mLoadBalancingClient.opRaiseEvent(true, data, 8, EventCode::BULLET_POS);
}

void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	if (eventCode == EventCode::GAME_STATE)
	{
		unsigned char* data = ExitGames::Common::ValueObject<nByte*>(eventContent).getDataCopy();
		if (data)
		{
			UIntPacker::getInstance().SetData(data[0]);
			unsigned int restart = UIntPacker::getInstance().extract(1);
			unsigned int exit = UIntPacker::getInstance().extract(1);
			std::cout << "Restart" << restart << std::endl;
			std::cout << "Exit" << exit << std::endl;
			if (exit == 1)
			{
				ExitGameWorld();
			}
			else if (restart == 1)
			{
				RestartGame();
			}
			
		}
		else
		{
			std::cout << "invalid data: GAME_STATE" << std::endl;
		}
	}

	if (gameData->canPlay == true)
	{
		static bool GotBulletID = false;
		static bool GotBulletType = false;
		static bool GotBulletPos = false;
		static unsigned int bulletID;
		static unsigned int bulletType;
		
		if (eventCode == EventCode::PLAYER_POS)
		{
			float* data = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
			if (data)
			{
				OnReceivedOpponentData(data);
			}
			else
			{
				std::cout << "invalid data: PLAYER_POS" << std::endl;
			}
		}

		if (eventCode == EventCode::BULLET_ID)
		{
			unsigned char* tempBulletID = ExitGames::Common::ValueObject<nByte*>(eventContent).getDataCopy();
			if (tempBulletID)
			{
				bulletID = *tempBulletID;
				GotBulletID = true;
			}
			else
			{
				std::cout << "invalid data: BULLET_ID" << std::endl;
			}
		}

		if (eventCode == EventCode::BULLET_TYPE)
		{
			unsigned char* tempBulletType = ExitGames::Common::ValueObject<nByte*>(eventContent).getDataCopy();
			if (tempBulletType)
			{
				bulletType = *tempBulletType;
				GotBulletType = true;
			}
			else
			{
				std::cout << "invalid data: BULLET_TYPE" << std::endl;
			}
		}

		if (eventCode == EventCode::BULLET_POS)
		{
			float* tempBulletPos = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
			if (tempBulletPos)
			{
				GotBulletPos = true;
				if (GotBulletID == true && GotBulletPos == true && GotBulletType == true)
				{
					unsigned int id = (int)bulletID;
					OnReceivedBulletData(id, bulletType, tempBulletPos);
					GotBulletID = false;
					GotBulletPos = false;
					GotBulletType = true;
				}
			}
			else
			{
				std::cout << "invalid data: BULLET_POS" << std::endl;
			}
		}
	}
	else
	{
		if (eventCode == EventCode::PLAYER_NUM)
		{
			std::cout << "PLAYER NUM" << std::endl;

			unsigned char* data = ExitGames::Common::ValueObject<nByte*>(eventContent).getDataCopy();
			if (data)
			{
				int dataNum = *data;
				if (dataNum > gameData->turnNum)
				{
					std::cout << dataNum << std::endl;
					playerData->playerType = PlayerType::PLAYER_2;
					enemyData->playerType = PlayerType::PLAYER_1;
					InitializePlayerEnemyData();
					gameData->gameState = GameState::GAME_WORLD;
					gameData->canPlay = true;
				}
				else if (dataNum < gameData->turnNum)
				{
					std::cout << dataNum << std::endl;;
					playerData->playerType = PlayerType::PLAYER_1;
					enemyData->playerType = PlayerType::PLAYER_2;
					InitializePlayerEnemyData();
					gameData->gameState = GameState::GAME_WORLD;
					gameData->canPlay = true;
				}
				else
				{
					sendTurnNum();
				}
			}
			else
			{
				std::cout << "invalid data" << std::endl;
			}
		}

		
	}
}