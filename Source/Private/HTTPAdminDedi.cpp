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
		ReturnMSG += TEXT("{\"servType\":\"Dedi\"}");

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

	return true;
}