Supported UE4 Versions
-------------

UE4.12
UE4.11
UE4.10

NOTE: It is recommended to upgrade to UE4.13+ which will have this plugin already included with the engine.

Installation
-------------

1) Close the Unreal Editor
2) Extract zip to the Plugins Folder of Unreal: https://docs.unrealengine.com/latest/INT/Programming/Plugins/index.html#pluginfolders


Configuration
-------------
1) Open DefaultEngine.ini in their project
2) Add the following lines:

	[OnlineSubsystem]
	DefaultPlatformService=Oculus

	[OnlineSubsystemOculus]
	bEnabled=true
	OculusAppId=<your_app_id>
  
	[/Script/Engine.GameEngine]
	+NetDriverDefinitions=(DefName="GameNetDriver",DriverClassName="OnlineSubsystemOculus.OculusNetDriver",DriverClassNameFallback="OnlineSubsystemUtils.IpNetDriver")

	[/Script/OnlineSubsystemOculus.OculusNetDriver]
	NetConnectionClassName="OnlineSubsystemOculus.OculusNetConnection"

	[/Script/Engine.NetDriver]
	AllowPeerConnections=False
	AllowPeerVoice=False
	ConnectionTimeout=60.0
	InitialConnectTimeout=60.0
	KeepAliveTime=0.2
	MaxClientRate=15000
	MaxInternetClientRate=10000
	RelevantTimeout=5.0
	SpawnPrioritySeconds=1.0
	ServerTravelPause=4.0
	NetServerMaxTickRate=30
	MaxPortCountToTry=512

3) In your <project_name>.Build.cs add OnlineSubsystem (OSS) as a dependency:

	PrivateDependencyModuleNames.Add("OnlineSubsystem");
  
4) [UE4.12.5] Before calling any Online or IOnlineSubsystem functions, add the following to your *.cpp

  #include "ModuleManager.h"
  ...
  if (IsRunningCommandlet()) 
  {
    FModuleManager::Get().LoadModule(TEXT("OnlineSubsystem"));
  }

This will ensure that the OnlineSubsystem module is loaded when cooking content for windows.
	

OSS Interfaces Provided
-------------

- Achievements
- Friends
- Identity
- Session


Contact
-------------

Direct any questions to our forums: https://forums.oculus.com/developer/categories/unreal