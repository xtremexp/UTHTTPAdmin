#pragma once

#include "HTTPAdmin.h"
//#include "HTTPAdminDedi.generated.h"


class HTTPAdminDedi : public HTTPAdminCommon {

public:
	FString GetJSONReturn();
	void SetGameMode(AUTGameMode* GameMode);
	void SetConnection(mg_connection* conn);
	bool ProcessRequest();

//protected:
	bool ProcessGet();
	bool ProcessPost();
//private:
	FString ReturnMSG = "";
	AUTGameMode* GameMode;
	mg_connection* conn;

	FString RequestServerInfo();
	FString URL;

};
