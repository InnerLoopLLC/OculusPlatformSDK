// Copyright 2016 Oculus VR, LLC All Rights reserved.

#pragma once

#include "GameFramework/Actor.h"
#include "OSSLeaderboardOculusActor.generated.h"

UCLASS()
class OCULUSPLATFORMSAMPLE_API AOSSLeaderboardOculusActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOSSLeaderboardOculusActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintImplementableEvent, Category = Leaderboards)
		void OnLeaderBoardInfoReceived(const FString& LBName);

	//virtual void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) override;

	//void OnPrivilegeCheck(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 CheckResult);
	
private:
	FString LBName;


};
