// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#include "HTTPAdmin.h"
#include "Base64.h"
#include "UTGameEngine.h"
// REMOVE once done
#include "UTLobbyGameState.h"
#include "UTLobbyGameMode.h"

UHTTPAdmin::UHTTPAdmin(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	MGServer = nullptr;
	GameMode = nullptr;
}

void UHTTPAdmin::Init()
{
	// Don't garbage collect me
	SetFlags(RF_RootSet);

	CombinedAuth = User + TEXT(":") + Password;

	if (Port == 0)
	{
		Port = 8080;
	}

	// Start Mongoose with all requests being handeled by 'StaticMGHandler'
	MGServer = mg_create_server(this, StaticMGHandler);
	
	// Set the port
	FString PortStr = FString::FromInt(Port);
	mg_set_option(MGServer, "listening_port", TCHAR_TO_ANSI(*PortStr));

	// Set the document root
	FString DocumentRoot = FPaths::GamePluginsDir() / TEXT("HTTPAdmin") / TEXT("HTML");
	mg_set_option(MGServer, "document_root", TCHAR_TO_ANSI(*DocumentRoot));

	// Set access log file
	//FString AccessLog = FPaths::GamePluginsDir() / TEXT("HTTPAdmin") / TEXT("accessLog.txt");
	//mg_set_option(MGServer, "access_log_file", TCHAR_TO_ANSI(*AccessLog));

	// http://www.askapache.com/online-tools/htpasswd-generator/
	// Set the auth password file
	// DEFAULT: admin  admin
	FString AuthFile = FPaths::GamePluginsDir() / TEXT("HTTPAdmin") / TEXT(".htpasswd");
	mg_set_option(MGServer, "global_auth_file", TCHAR_TO_ANSI(*AuthFile));

	// Set the auth domain
	FString AuthDomain = TEXT("HTTPAdmin");
	mg_set_option(MGServer, "auth_domain", TCHAR_TO_ANSI(*AuthDomain));
	

	UE_LOG(HTTPAdmin, Log, TEXT("=================="));
	UE_LOG(HTTPAdmin, Log, TEXT("HTTPAdmin Started on port: %i"), Port);
	UE_LOG(HTTPAdmin, Log, TEXT("=================="));
}

void UHTTPAdmin::Tick(float DeltaTime)
{
	if (MGServer != nullptr)
	{
		mg_poll_server(MGServer, 1);
	}
}

TStatId UHTTPAdmin::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UHTTPAdmin, STATGROUP_Tickables);
}

int UHTTPAdmin::StaticMGHandler(mg_connection* conn, enum mg_event ev)
{
	return ((UHTTPAdmin*)conn->server_param)->MGHandler(conn, ev);
}

int UHTTPAdmin::MGHandler(mg_connection* conn, enum mg_event ev)
{
	if (ev == MG_AUTH)
	{
		return MG_TRUE;   // Authorize any requests
	}
	else if (ev == MG_REQUEST) // Something was requested
	{
		// Determin if this is a Lobby or Dedi server
		// Create a new object of given type
		// Call ProcessRequest
		// Call JSON return
		// Send return back to client

		// Get a reference of any object using the UTBaseGameMode
		AUTBaseGameMode* BaseGameMode;
		BaseGameMode = Cast<AUTBaseGameMode>(GWorld->GetAuthGameMode());

		// Are we a Lobby server?
		if (BaseGameMode->IsLobbyServer())
		{
			// Yes, get a reference to the lobby 
			AUTLobbyGameMode* LobbyGameMode = GWorld->GetAuthGameMode<AUTLobbyGameMode>();
			if (LobbyGameMode)
			{
				// Create a new object
				HTTPAdminLobby* HTTPLobby;
				HTTPLobby->GameMode = LobbyGameMode;
				// set World
				// set GameMode
				// set Connection
				// IF object->ProcessRequest
					// Call JSON return
				// else
					// return MG_FALSE; // Nothing for us to process here, hand back to Mongoose to surve page/image

			}
			else
			{
				// Umm error, we are not a lobby server
			}
		} 
		else
		{
			// Nope, we must be a dedi server

			AUTGameMode* GameMode = GWorld->GetAuthGameMode<AUTGameMode>();
			if (GameMode)
			{
				// Create a new object
				// Call ProcessRequest
				// Call JSON return
			}
			else
			{
				// Umm error, we are not a dedi server
			}

		}





		// Send return back to client










		// -----------







		

		GameMode = Cast<AUTGameMode>(GWorld->GetAuthGameMode());

		//AUTLobbyGameState* LobbyGameState = GWorld->GetGameState<AUTLobbyGameState>();

		FString URL(conn->uri);

		const FString FileExtension = FPaths::GetExtension(URL);
		const FString FileName = FPaths::GetBaseFilename(URL);
		const FString FullFileName = FPaths::GetCleanFilename(URL);

		// GET
		if (FString(conn->request_method) == FString(TEXT("GET")))
		{
			// test.json
			if (FullFileName == FString(TEXT("test.json")))
			{
				FString Message;
				/*
				if (LobbyGameState)
				{
					Message = TEXT("Lobby");
					if (GameMode) {
						Message += TEXT("+ Game Mode");
					}
				}
				else
				{
					Message = TEXT("Dedi");
					if (GameMode) {
						Message += TEXT("+ Game Mode");
					}
				}
				*/
				/*
				if (GameMode2->IsLobbyServer())
				{
					Message = TEXT("Lobby");
				} 
				else
				{
					Message = TEXT("Dedi");
				}
				*/

				mg_send_header(conn, "Content-Type", "application/json");
				mg_send_data(conn, TCHAR_TO_ANSI(*Message), Message.Len());
				return MG_TRUE; // Request has been processed, no need to hand back to Mongoos
			}

			// request.json
			if (FullFileName == FString(TEXT("request.json")))
			{
				FString Message = FString(TEXT("Hello, this was a request to a json"));
				
				mg_send_header(conn, "Content-Type", "application/json");
				mg_send_data(conn, TCHAR_TO_ANSI(*Message), Message.Len());
				return MG_TRUE; // Request has been processed, no need to hand back to Mongoos
			}

			// DEBUG: Respond to any *.json request
			if (FileExtension == FString(TEXT("json")))
			{
				//
				FString JSON = PrepareAdminJSON();

				mg_send_header(conn, "Content-Type", "application/json");
				mg_send_data(conn, TCHAR_TO_ANSI(*JSON), JSON.Len());
				return MG_TRUE; // Request has been processed, no need to hand back to Mongoose
			}

		} // GET

		// POST
		if (FString(conn->request_method) == FString(TEXT("POST")))
		{
			if (FullFileName == FString(TEXT("action.json")))
			{
				// Check if any data was POSTed to us
				if (conn->content_len > 0)
				{
					// Try and decode the JSON string sent
					FString JsonString = ANSI_TO_TCHAR((char*)conn->content);

					TSharedPtr<FJsonValue> JsonObject;
					TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

					// Attempt to decode the JSON string into usable objects
					// error C2665: 'FJsonSerializer::Deserialize' : none of the 3 overloads could convert all the argument types
					//FJsonSerializer::Deserialize(JsonReader, JsonObject);
					
					// Was the decode successful?
					if (JsonObject.IsValid())
					{
						// loop actions
						//     Check for method == to requested action
						//     if found call method passing data
						//        method returns status for response
						//     if method not found
						//        return status for response
						// send response to client

					}
					else
					{
						// Error: request not in the correct format
					}

					// DEBUG: send "Done" back to the client
					FString Message = FString("Done");
					//Message += conn->content;
					mg_send_header(conn, "Content-Type", "application/json");
					mg_send_data(conn, TCHAR_TO_ANSI(*Message), Message.Len());
					return MG_TRUE; // Request has been processed, no need to hand back to Mongoose
				}
				else
				{
					// No data wihtin the POST request
					return MG_FALSE;
				} 
			} // action.json
		} // POST
	} // request

	return MG_FALSE; // Nothing for us to process here, hand back to Mongoose to surve page/image
}


FString UHTTPAdmin::PrepareAdminJSON()
{
	
	FString JSON;

	if (GameMode != nullptr && GameMode->GameState != nullptr)
	{
		JSON += TEXT("{");

		JSON += TEXT("\"gamemode\":\"") + GameMode->GetName() + TEXT("\",");
		JSON += TEXT("\"mapname\":\"") + GWorld->GetMapName() + TEXT("\",");

		JSON += TEXT("\"maprotation\":[");
		for (int i = 0; i < GameMode->MapRotation.Num(); i++)
		{
			if (i != 0)
			{
				JSON += TEXT(",");
			}
			JSON += TEXT("\"") + GameMode->MapRotation[i] + TEXT("\"");
		}
		JSON += TEXT("],");

		
		JSON += TEXT("\"timelimit\":") + FString::FromInt(GameMode->TimeLimit) + TEXT(",");
		JSON += TEXT("\"timeremaining\":") + FString::FromInt(GameMode->UTGameState->RemainingTime) + TEXT(",");

		JSON += TEXT("\"teamGame\":") + FString::FromInt(GameMode->bTeamGame) + TEXT(",");

		JSON += TEXT("\"MatchState\":\"") + FriendlyMatchState(GameMode->GetMatchState()) + TEXT("\",");

		JSON += TEXT("\"hasMatchStarted\":") + FString::FromInt(GameMode->HasMatchStarted()) + TEXT(",");
		JSON += TEXT("\"IsMatchInProgress\":") + FString::FromInt(GameMode->IsMatchInProgress()) + TEXT(",");
		

		JSON += TEXT("\"minPlayerToStart\":") + FString::FromInt(GameMode->MinPlayersToStart) + TEXT(",");
		
		JSON += TEXT("\"goalscore\":") + FString::FromInt(GameMode->UTGameState->GoalScore) + TEXT(",");

		// Handle Player request
		JSON += RequestJSONPlayers();

		//JSON += TEXT("}, {");
		//JSON += TEXT("\"ServerName\":\"") + GameMode->UTGameState->ServerName + TEXT("\",");
		//JSON += TEXT("\"ServerMOTD\":\"") + GameMode->UTGameState->ServerMOTD + TEXT("\"");

		JSON += TEXT("}");
	}

	return JSON;
}

/**
Returns player information as a JSON string

"Players":[
	{
		"name": ,   // Player Name
		"id": ,     // Player Id
		"bot": ,    // Is the player a bot?
		"kills": ,  // Number of kills
		"deaths":,  // Number od deaths
		"score":,   // Player score
		"team": ,   // Player team number
		"flagCaptures":	,  // CTF: Number of flag captures
		"flagReturns": ,   // CTF: Number of flag returns
		"flagAssists":	   // CTF: Number of assisted flag captures
	},
]

*/
FString UHTTPAdmin::RequestJSONPlayers()
{

	FString JSON;

	//JSON +=	TEXT("{");

	JSON += TEXT("\"Players\":[");
	for (int i = 0; i < GameMode->GameState->PlayerArray.Num(); i++)
	{
		if (i != 0)
		{
			JSON += TEXT(",");
		}
		AUTPlayerState *PS = Cast<AUTPlayerState>(GameMode->GameState->PlayerArray[i]);
		if (PS != nullptr)
		{
			// StartTime
			// bIsABot
			JSON += TEXT("{");
			JSON += TEXT("\"name\":\"") + PS->PlayerName + TEXT("\",");
			JSON += TEXT("\"id\":\"") + FString(PS->StatsID) + TEXT("\",");
			JSON += TEXT("\"bot\":\"") + FString::FromInt(PS->bIsABot) + TEXT("\",");
			JSON += TEXT("\"kills\":") + FString::FromInt(PS->Kills) + TEXT(",");
			JSON += TEXT("\"deaths\":") + FString::FromInt(PS->Deaths) + TEXT(",");
			JSON += TEXT("\"score\":") + FString::FromInt(PS->Score) + TEXT(",");

			if (PS->Team != nullptr)
			{
				JSON += TEXT("\"team\":") + FString::FromInt(PS->Team->TeamIndex) + TEXT("");
			}
			else
			{
				JSON += TEXT("\"team\":\"na\"");
			}
			
			if (GameMode->GetName().StartsWith("UTCTFGameMode"))
			{
				JSON += TEXT(",");
				JSON += TEXT("\"flagCaptures\":") + FString::FromInt(PS->FlagCaptures) + TEXT(",");
				JSON += TEXT("\"flagReturns\":") + FString::FromInt(PS->FlagReturns) + TEXT(",");
				JSON += TEXT("\"flagAssists\":") + FString::FromInt(PS->Assists) + TEXT("");
			}

			JSON += TEXT("}");
		}
	}
	JSON += TEXT("]");

	//JSON += TEXT("}");

	return JSON;
}

// Kick given player (name or id) from the server
FString UHTTPAdmin::ActionKick(FString data)
{
	// NameOrUID
	return FString("Player Kicked");
}

// Ben the given player (name or id) from the server
FString UHTTPAdmin::ActionBen(FString data)
{
	// NameOrUID
	return FString("Player Banned");
}

// Run given console command
FString UHTTPAdmin::ActionConsoleCommend(FString data)
{
	/*
	char argumentbuffer[256];
	argumentbuffer[255] = 0;
	if (mg_get_var(conn, "consolecommand", argumentbuffer, 255) > 0)
	{
	FString ConsoleCommandString(argumentbuffer);
	GEngine->Exec(GWorld, *ConsoleCommandString);
	}
	*/

	return FString("Command Executed");
}


// Converts the MatchState into a usable string
FString UHTTPAdmin::FriendlyMatchState(FName MatchState)
{
	// Engine GameModes
	if (MatchState == MatchState::WaitingToStart) // Actors are ticking, but the match has not yet started
		return FString(TEXT("Waiting To Start"));

	if (MatchState == MatchState::InProgress) // Normal gameplay is occurring
		return FString(TEXT("In Process"));

	if (MatchState == MatchState::WaitingPostMatch) // Match has ended so we aren't accepting new players, but actors are still ticking
		return FString(TEXT("Waiting Post Match"));

	if (MatchState == MatchState::LeavingMap) // // We are transitioning out of the map to another location
		return FString(TEXT("Leaving Map"));

	if (MatchState == MatchState::Aborted) //Match has failed due to network issues or other problems, cannot continue
		return FString(TEXT("Aborted"));

	// UT GameModes
	if (MatchState == MatchState::PlayerIntro) // Playing the player intro in the match summary window
		return FString(TEXT("Player Intro"));

	if (MatchState == MatchState::CountdownToBegin) // We are entering this map, actors are not yet ticking
		return FString(TEXT("Countdown To Begin"));

	if (MatchState == MatchState::MatchEnteringOvertime) // The gane is entering overtime
		return FString(TEXT("Entering Overtime"));
	
	if (MatchState == MatchState::MatchIsInOvertime) // The game is in overtime
		return FString(TEXT("Overtime"));

	if (MatchState == MatchState::MapVoteHappening) // The game is in mapvote stage
		return FString(TEXT("Map Vote"));

	if (MatchState == MatchState::MatchIntermission) // The game is in a round intermission
		return FString(TEXT("Intermission"));

	// UT CTF
	if (MatchState == MatchState::MatchEnteringHalftime) // CTF game entering half time
		return FString(TEXT("Entering Half Time"));

	if (MatchState == MatchState::MatchIsAtHalftime) // CTF game at half time
		return FString(TEXT("At Half Time"));

	if (MatchState == MatchState::MatchExitingHalftime) // CTF game half time eneded
		return FString(TEXT("Exiting Half Time"));

	return FString(TEXT("Unknown"));
	
}

void SetServType()
{
//	if (GWorld->Get)
	// ServType = ServType::HUB;
	//ServType = ServType::Dedi;
	//ServType = ServType::Unknown;
}