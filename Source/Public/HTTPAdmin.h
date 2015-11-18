// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Core.h"
#include "UnrealTournament.h"
#include "UTCTFGameMode.h"
#include "Json.h"

// include Mongoose, issues with DWORD
#include "AllowWindowsPlatformTypes.h"
#include "mongoose.h"
#include "HideWindowsPlatformTypes.h"

#include "HTTPAdminCommon.h"
#include "HTTPAdminLobby.h"
#include "HTTPAdminDedi.h"

#include "HTTPAdmin.generated.h"

// Log messages
DEFINE_LOG_CATEGORY_STATIC(HTTPAdmin, Log, All);


namespace ServType
{
	const FName Lobby = FName(TEXT("Lobby"));		// Hub/Lobby
	const FName Dedi = FName(TEXT("Dedi"));			// Dedicated
	const FName Unknown = FName(TEXT("Unkown"));			// Unkown type
}


// Load the HTTPAdmin config
UCLASS(Config=HTTPAdmin)
class UHTTPAdmin : public UObject, public FTickableGameObject
{

	GENERATED_UCLASS_BODY()

	void Init();

	// FTickableGameObject interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override
	{
		return true;
	}
	virtual bool IsTickableWhenPaused() const override
	{
		return true;
	}

	// Mongoose
	static int StaticMGHandler(mg_connection* conn, enum mg_event ev);	
	int MGHandler(mg_connection* conn, enum mg_event ev);

	// Other Methods
	FString PrepareAdminJSON();
	static FString FriendlyMatchState(FName MatchState);
	FString RequestJSONPlayers();

	FString ActionKick(FString data);
	FString ActionBen(FString data);
	FString ActionConsoleCommend(FString data);

	// Server Type (ServType)
	FName ServType;
	void SetServType();

	// Get config settings
	UPROPERTY(Config)
	bool bRequireAuth;

	UPROPERTY(Config)
	FString User;

	UPROPERTY(Config)
	FString Password;

	FString CombinedAuth;

	UPROPERTY(Config)
	uint32 Port;

private:
	mg_server* MGServer;
	AUTGameMode* GameMode;

};