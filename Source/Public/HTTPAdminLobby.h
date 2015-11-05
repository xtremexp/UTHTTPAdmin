#pragma once

#include "HTTPAdmin.h"
#include "UTLobbyGameState.h"
#include "UTLobbyGameMode.h"

//#include "HTTPAdminLobby.generated.h"

class HTTPAdminLobby : public HTTPAdminCommon {

public:
	FString GetJSONReturn();
	void SetGameMode(AUTLobbyGameMode GameMode);

//private:
	AUTLobbyGameMode* GameMode;

};