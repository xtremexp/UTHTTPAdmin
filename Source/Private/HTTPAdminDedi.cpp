#include "HTTPAdmin.h"

#include "HTTPAdminCommon.h"
#include "HTTPAdminDedi.h"

FString HTTPAdminDedi::GetJSONReturn()
{
	return ReturnMSG;
}

void HTTPAdminDedi::SetGameMode(AUTGameMode* UTGameMode)
{
	HTTPAdminDedi::GameMode = GameMode;
}

void HTTPAdminDedi::SetConnection(mg_connection* conn)
{
	HTTPAdminDedi::conn = conn;
}

bool HTTPAdminDedi::ProcessRequest()
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

bool HTTPAdminDedi::ProcessGet()
{
	FString URL(conn->uri);

	const FString FileExtension = FPaths::GetExtension(URL);
	const FString FileName = FPaths::GetBaseFilename(URL);
	const FString FullFileName = FPaths::GetCleanFilename(URL);

	// request.json
	if (FullFileName == FString(TEXT("request.json")))
	{
		//ReturnMSG += TEXT("Hello, this was a request to the lobby json");
		ReturnMSG += TEXT("{\"servType\":\"dedi\"}");

		return true; // Request has been processed
	}

	return false; // There is nothing for us to process
}

bool HTTPAdminDedi::ProcessPost()
{
	FString URL(conn->uri);

	const FString FileExtension = FPaths::GetExtension(URL);
	const FString FileName = FPaths::GetBaseFilename(URL);
	const FString FullFileName = FPaths::GetCleanFilename(URL);

	// Actions e.g. Set map, kick player
	if (FullFileName == FString(TEXT("action.json")))
	{
	}

	//----------------



	// Requested information, e.g. Server Name, Current map, players
	if (FullFileName == FString(TEXT("request.json")))
	{
		// Check if any data was POSTed to us
		if (conn->content_len > 0)
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
						Response->SetStringField("data", RequestServerInfo()); // Need to pass the data to called methof

						// Add this response to the list of responses
						ResponcesJsonObj.AddZeroed();
						ResponcesJsonObj[Idx] = MakeShareable(new FJsonValueObject(Response));
					}


				}


				ReturnJsonObj->SetArrayField(TEXT("responce"), ResponcesJsonObj);
				

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
		else
		{
			// Error: no data posted
			ReturnMSG += "Error: no data posted";
			return true; // false
		}
	}

	return false; // There is nothing for us to process
}

FString HTTPAdminDedi::RequestServerInfo()
{
	return FString(TEXT("{\"serverName\":\"na\"}, {\"motd\":\"na\"}"));
}