#pragma once

#include "HTTPAdmin.h"
#include "UTLobbyGameState.h"
#include "UTLobbyGameMode.h"

//#include "HTTPAdminLobby.generated.h"

class HTTPAdminLobby : public HTTPAdminCommon {

public:
	FString GetJSONReturn();
	void SetGameMode(AUTLobbyGameMode* GameMode);
	void SetConnection(mg_connection* conn);
	bool ProcessRequest();
//protected:
	bool ProcessGet();
	bool ProcessPost();

//private:
	FString ReturnMSG = "";
	AUTLobbyGameMode* GameMode;
	mg_connection* conn;

};