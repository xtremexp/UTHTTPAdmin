#include "HTTPAdmin.h"

#include "HTTPAdminCommon.h"
#include "HTTPAdminDedi.h"

FString HTTPAdminDedi::GetJSONReturn()
{

	return FString(TEXT("Dedi Return"));
}

void HTTPAdminDedi::SetGameMode(AUTGameMode UTGameMode)
{
	//HTTPAdminDedi::GameMode = UTGameMode;
}