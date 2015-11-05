#pragma once

#include "HTTPAdmin.h"
//#include "HTTPAdminDedi.generated.h"


class HTTPAdminDedi : public HTTPAdminCommon {

public:
	FString GetJSONReturn();
	void SetGameMode(AUTGameMode GameMode);

//private:
	AUTGameMode* GameMode;

};