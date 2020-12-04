// Copyright 2016 Oculus VR, LLC All Rights reserved.

#include "OculusPlatformSample.h"
#include "OSSLeaderboardOculusActor.h"


// Sets default values
AOSSLeaderboardOculusActor::AOSSLeaderboardOculusActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOSSLeaderboardOculusActor::BeginPlay()
{
	Super::BeginPlay();
	OnLeaderBoardInfoReceived(TEXT("Not Yet Implemented!"));
	
}

// Called every frame
void AOSSLeaderboardOculusActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

