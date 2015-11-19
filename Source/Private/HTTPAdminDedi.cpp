#include "HTTPAdmin.h"

#include "HTTPAdminCommon.h"
#include "HTTPAdminDedi.h"

HTTPAdminDedi::HTTPAdminDedi(mg_connection* conn)
{
	GameMode = GWorld->GetAuthGameMode<AUTGameMode>();
	HTTPAdminDedi::conn = conn;
}

FString HTTPAdminDedi::GetJSONReturn()
{
	return ReturnMSG;
}

bool HTTPAdminDedi::ProcessRequest()
{
	// Get the url being requested
	FString URL(conn->uri);
	HTTPAdminDedi::URL = URL;
	// Notes:
	// FileExtension = FPaths::GetExtension(URL);
	// FileName = FPaths::GetBaseFilename(URL);
	// FullFileName = FPaths::GetCleanFilename(URL);

	// GET
	if (FString(conn->request_method) == FString(TEXT("GET")))
	{
		return ProcessGet();
	}

	// POST
	if (FString(conn->request_method) == FString(TEXT("POST")))
	{
		// Check if any data was POSTed to us
		if (conn->content_len > 0)
			return ProcessPost();
		else
			return false; // Error: no data posted
	}

	return false; // Request method was something other then GET or POST
}

bool HTTPAdminDedi::ProcessGet()
{
	// request.json
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("request.json")))
	{
		//ReturnMSG += TEXT("Hello, this was a request to the lobby json");
		ReturnMSG += TEXT("{\"servType\":\"dedi\"}");

		return true; // Request has been processed
	}

	return false; // There is nothing for us to process
}

bool HTTPAdminDedi::ProcessPost()
{

	// Actions e.g. Set map, kick player
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("action.json")))
	{
	}

	//----------------



	// Requested information, e.g. Server Name, Current map, players
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("request.json")))
	{

		// Get the data which was posted
		FString JsonString = FString(ANSI_TO_TCHAR(conn->content));
		// We only want the request body
		JsonString = JsonString.Left((int)conn->content_len);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

		// Attempt to decode the JSON string into usable objects
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

		// Was the decode successful?
		if (JsonObject.IsValid())
		{

			// REF: return json
			// TODO: move this var to global
			TSharedPtr<FJsonObject> ReturnJsonObj = MakeShareable(new FJsonObject);
			TArray<TSharedPtr<FJsonValue>> ResponcesJsonObj;



			TArray< TSharedPtr<FJsonValue> > RequestList = JsonObject->GetArrayField(TEXT("request"));
			// loop
			for (int32 Idx = 0; Idx < RequestList.Num(); Idx++)
			{
				TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject);


				TSharedPtr<FJsonObject> RequestItem = RequestList[Idx]->AsObject();
				// Get the request name
				FString RequestName = RequestItem->GetStringField(TEXT("name"));

				// Get any data which was posted with this request
				const TArray< TSharedPtr<FJsonValue> >* RequestData;
				RequestItem->TryGetArrayField(TEXT("data"), RequestData);

				// Action the requests

				if (RequestName == "serverInfo")
				{
					Response->SetStringField("name", RequestName);
					Response->SetObjectField("data", RequestServerInfo()); // Need to pass the data to called method
				}

				if (RequestName == "matchInfo")
				{
					Response->SetStringField("name", RequestName);
					Response->SetObjectField("data", RequestMatchInfo()); // Need to pass the data to called method
				}

				if (RequestName == "players")
				{
					Response->SetStringField("name", RequestName);
					Response->SetArrayField("data", RequestPlayers()); // Need to pass the data to called method
				}

				// Add this response to the list of responses
				ResponcesJsonObj.AddZeroed();
				ResponcesJsonObj[Idx] = MakeShareable(new FJsonValueObject(Response));

			}

			ReturnJsonObj->SetStringField(TEXT("servType"), TEXT("dedi"));
			ReturnJsonObj->SetArrayField(TEXT("response"), ResponcesJsonObj);
				

			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);

			FJsonSerializer::Serialize(ReturnJsonObj.ToSharedRef(), Writer);
			ReturnMSG += OutputString;

			return true; // Request has been processed

		}
		else
		{
			// Error: request not in the correct format
			ReturnMSG += "Error: request not in the correct format";
			return true; // false
		}
	}

	return false; // There is nothing for us to process
}

TSharedPtr<FJsonObject> HTTPAdminDedi::RequestServerInfo()
{
	TSharedPtr<FJsonObject> ServerInfo = MakeShareable(new FJsonObject);

	if (GameMode != nullptr && GameMode->GameState != nullptr)
	{

		ServerInfo->SetStringField(TEXT("serverName"), GameMode->UTGameState->ServerName); // GameMode->UTGameState->ServerName
		ServerInfo->SetStringField(TEXT("motd"), GameMode->UTGameState->ServerMOTD);	// GameMode->UTGameState->ServerMOTD
		ServerInfo->SetStringField(TEXT("gameMode"), GameMode->GetName()); // GameMode->GetName()
		ServerInfo->SetStringField(TEXT("mapRotation"), TEXT("na"));
	}
	else
	{
		ServerInfo->SetStringField(TEXT("error"), TEXT("no game mode or game state"));
	}
	return ServerInfo;
}

TSharedPtr<FJsonObject> HTTPAdminDedi::RequestMatchInfo()
{
	int32 PlayerCount = 0;
	int32 BotCount = 0;
	int32 SpecCount = 0;

	TSharedPtr<FJsonObject> MatchInfo = MakeShareable(new FJsonObject);

	MatchInfo->SetStringField(TEXT("map"), GWorld->GetMapName());
	MatchInfo->SetNumberField(TEXT("timeLimit"), GameMode->TimeLimit );
	MatchInfo->SetNumberField(TEXT("timeRemaining"), GameMode->UTGameState->RemainingTime);
	MatchInfo->SetNumberField(TEXT("teamGame"), GameMode->bTeamGame);
	MatchInfo->SetStringField(TEXT("state"), UHTTPAdmin::FriendlyMatchState(GameMode->GetMatchState())); // TODO: Move FriendlyMatchState to common?
	MatchInfo->SetNumberField(TEXT("hasStarted"), GameMode->HasMatchStarted()); 
	MatchInfo->SetNumberField(TEXT("InProgress"), GameMode->IsMatchInProgress()); 
	MatchInfo->SetNumberField(TEXT("goalScore"), GameMode->UTGameState->GoalScore); 
	MatchInfo->SetNumberField(TEXT("minPlayersToStart"), GameMode->MinPlayersToStart); 

	MatchInfo->SetNumberField(TEXT("maxPlayers"), GameMode->GameSession->MaxPlayers);
	MatchInfo->SetNumberField(TEXT("maxSpectators"), GameMode->GameSession->MaxSpectators);

	MatchInfo->SetNumberField(TEXT("playerCount"), GameMode->NumPlayers);
	MatchInfo->SetNumberField(TEXT("botCount"), GameMode->NumBots);
	MatchInfo->SetNumberField(TEXT("specCount"), GameMode->NumSpectators);


	return MatchInfo;
}

TArray< TSharedPtr<FJsonValue> > HTTPAdminDedi::RequestPlayers()
{
	TArray< TSharedPtr<FJsonValue> > Players;

	for (int32 Idx = 0; Idx < GameMode->GameState->PlayerArray.Num(); Idx++)
	{
		AUTPlayerState *PS = Cast<AUTPlayerState>(GameMode->GameState->PlayerArray[Idx]);
		if (PS != nullptr)
		{
			TSharedPtr<FJsonObject> Player = MakeShareable(new FJsonObject);
			Player->SetStringField(TEXT("name"), PS->PlayerName);
			Player->SetStringField(TEXT("id"), PS->StatsID);
			Player->SetNumberField(TEXT("isABot"), PS->bIsABot);
			Player->SetNumberField(TEXT("kills"), PS->Kills);
			Player->SetNumberField(TEXT("deaths"), PS->Deaths);
			Player->SetNumberField(TEXT("score"), PS->Score);


			Players.AddZeroed();
			Players[Idx] = MakeShareable(new FJsonValueObject(Player));
		}
	}

	//Players->SetArrayField();
	return Players;
}