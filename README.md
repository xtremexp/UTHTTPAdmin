UTHTTPAdmin
===========

HTTP Administration Plugin for UT

This is currently UNSAFE and a prototype using the Mongoose HTTP deamon

We make use of the Mongoose a GPL-licensed HTTP deamon from https://github.com/cesanta/mongoose

-- I'm in no way a programmer but I do tinker and move things around until it works --

Set up
===========
Get the latest UT release branch
Copy/Cone this to "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin"
Compile with Visual Studio using "Test Win64"

Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/HTML/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/HTML"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/.htpasswd" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/.htpasswd"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/Config/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/Config"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/Binaries/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/Binaries"

Add "Paths=../../../UnrealTournament/Plugins/HTTPAdmin/Content" under "[Core.System]" within your "Engine.inf" file ("My Documents/UnrealTournament/Saved/Config/WindowsNoEditor")

Start a dedicated server from the command promt e.g. 
cd "C:/Program Files/Epic Games/UnrealTournamentDev/Engine/Binaries/Win64"
UE4-Win64-Test.exe UnrealTournament DM-Outpost23 -server -log

Open a browser and go to http://localhost:8082 default login is admin/admin