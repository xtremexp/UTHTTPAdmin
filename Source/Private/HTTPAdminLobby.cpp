#include "HTTPAdmin.h"

#include "HTTPAdminCommon.h"
#include "HTTPAdminLobby.h"


FString HTTPAdminLobby::GetJSONReturn()
{
	return ReturnMSG;
}

void HTTPAdminLobby::SetGameMode(AUTLobbyGameMode* GameMode)
{
	HTTPAdminLobby::GameMode = GameMode;
}


void HTTPAdminLobby::SetConnection(mg_connection* conn)
{
	HTTPAdminLobby::conn = conn;
}

bool HTTPAdminLobby::ProcessRequest()
{
	// Get the url being requested
	FString URL(conn->uri);
	HTTPAdminLobby::URL = URL;
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


bool HTTPAdminLobby::ProcessGet()
{

	// request.json
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("request.json")))
	{
		//ReturnMSG += TEXT("Hello, this was a request to the lobby json");
		ReturnMSG += TEXT("{\"servType\":\"lobby\"}");
		
		return true; // Request has been processed
	}

	return false; // There is nothing for us to process
}

bool HTTPAdminLobby::ProcessPost()
{

	// Actions e.g. Set map, kick player
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("action.json")))
	{
		// Try and decode the JSON string sent
		FString JsonString = ANSI_TO_TCHAR((char*)conn->content);

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

		// Attempt to decode the JSON string into usable objects
		FJsonSerializer::Deserialize(JsonReader, JsonObject);
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
			return false;
		}

	}


	// --------------------------

	// Requested information, e.g. Server Name, Current map, players
	if (FPaths::GetCleanFilename(URL) == FString(TEXT("request.json")))
	{

		// Get the data which was posted
		FString JsonString = FString(ANSI_TO_TCHAR(conn->content));
		// We only want the request body
		JsonString = JsonString.Left((int)conn->content_len);

		// TODO:
		ReturnMSG += TEXT("{\"response\":[");

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

		// Attempt to decode the JSON string into usable objects
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

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
			TArray< TSharedPtr<FJsonValue> > RequestList = JsonObject->GetArrayField(TEXT("request"));
			for (int32 Idx = 0; Idx < RequestList.Num(); Idx++)
			{
				TSharedPtr<FJsonObject> RequestItem = RequestList[Idx]->AsObject();

				FString RequestName = RequestItem->GetStringField(TEXT("name"));

				const TArray< TSharedPtr<FJsonValue> >* RequestData;
				RequestItem->TryGetArrayField(TEXT("data"), RequestData);
					

				if (RequestName == "serverInfo")
				{
					ReturnMSG += TEXT("{\"name\":\"serverInfo\", \"data\": {}");
					//ReturnMSG += RequestServerInfo((FJsonValue) RequestData->AsString());
					ReturnMSG += TEXT("}");
				}

			}

			// TODO:
			ReturnMSG += TEXT("]}");
			return true;
		}
		else
		{
			// Error: request not in the correct format
			ReturnMSG += TEXT("Error with json format");
			return true;
		}

	}

	return false; // There is nothing for us to process
}

FString RequestServerInfo(FString RequestData)
{
	return FString(TEXT("{\"serverName\":\"na\"}, {\"motd\":\"na\"}"));
}