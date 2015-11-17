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

	if (FString(conn->request_method) == FString(TEXT("GET")))
	{
		return ProcessGet();
	}


	if (FString(conn->request_method) == FString(TEXT("POST")))
	{
		return ProcessPost();
	}

	return false;
}


bool HTTPAdminLobby::ProcessGet()
{

	FString URL(conn->uri);

	const FString FileExtension = FPaths::GetExtension(URL);
	const FString FileName = FPaths::GetBaseFilename(URL);
	const FString FullFileName = FPaths::GetCleanFilename(URL);

	// request.json
	if (FullFileName == FString(TEXT("request.json")))
	{
		//ReturnMSG += TEXT("Hello, this was a request to the lobby json");
		ReturnMSG += TEXT("{\"servType\":\"Lobby\"}");
		
		return true; // Request has been processed
	}

	return false; // There is nothing for us to process
}

bool HTTPAdminLobby::ProcessPost()
{

	FString URL(conn->uri);

	const FString FileExtension = FPaths::GetExtension(URL);
	const FString FileName = FPaths::GetBaseFilename(URL);
	const FString FullFileName = FPaths::GetCleanFilename(URL);

	// Actions e.g. Set map, kick player
	if (FullFileName == FString(TEXT("action.json")))
	{
		// Check if any data was POSTed to us
		if (conn->content_len > 0)
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
	}


	// --------------------------

	// Requested information, e.g. Server Name, Current map, players
	if (FullFileName == FString(TEXT("request.json")))
	{
		// Check if any data was POSTed to us
		if (conn->content_len > 0)
		{
			// Try and decode the JSON string sent


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
		else
		{
			ReturnMSG += TEXT("No data posted");
			return true;
		}
	}

	return false; // There is nothing for us to process
}

FString RequestServerInfo(FString RequestData)
{
	return FString(TEXT("{\"serverName\":\"na\"}, {\"motd\":\"na\"}"));
}