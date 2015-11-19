#pragma once

#include "HTTPAdmin.h"
//#include "HTTPAdminDedi.generated.h"


class HTTPAdminDedi : public HTTPAdminCommon {

public:
	HTTPAdminDedi(mg_connection* conn);
	FString GetJSONReturn();
	bool ProcessRequest();

//protected:

//private:
	FString ReturnMSG = "";
	AUTGameMode* GameMode;
	mg_connection* conn;
	FString URL;

	bool ProcessGet();
	bool ProcessPost();

	TSharedPtr<FJsonObject> RequestServerInfo();
	TSharedPtr<FJsonObject> RequestMatchInfo();
	TArray< TSharedPtr<FJsonValue> > RequestPlayers();

};
