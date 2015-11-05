#include "HTTPAdmin.h"

#include "HTTPAdminLobby.h"


FString HTTPAdminLobby::GetJSONReturn()
{

	return FString(TEXT("Lobby Return"));
}

void HTTPAdminLobby::SetGameMode(AUTLobbyGameMode* LobbyMode)
{
	//HTTPAdminLobby::GameMode = LobbyMode;
}