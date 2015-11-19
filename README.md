UTHTTPAdmin
===========

HTTP Administration Plugin for UT

This is currently UNSAFE and a prototype using the Mongoose HTTP deamon

We make use of the Mongoose a GPL-licensed HTTP deamon from https://github.com/cesanta/mongoose

-- I'm in no way a programmer but I do tinker and move things around until it works --

Set up
===========
Get the latest UT release branch
Extract/Clone this repository to ""<UnrealTournamentSource>/UnrealTournament/Plugins/"
Regenerate Visual Studio project
Compile with Visual Studio using Test Win64/Test Server Win64

Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/HTTPAdmin.uplugin" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/HTTPAdmin.uplugin"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/.htpasswd" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/.htpasswd"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/HTML/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/HTML"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/Config/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/Config"
Copy "<UnrealTournamentSource>/UnrealTournament/Plugins/HTTPAdmin/Binaries/*" to "<UnrealTournamentInstall>/UnrealTournament/Plugins/HTTPAdmin/Binaries"

Add the following under "[Core.System]" within "Engine.inf"
'''Paths=../../../UnrealTournament/Plugins/HTTPAdmin/Content'''

Start a dedicated server from the command promt e.g. 
"UnrealTournamentServer.exe UnrealTournament DM-Outpost23?Game=DM -log

Open a browser and go to http://<serverIP>:8082 default login is admin/admin