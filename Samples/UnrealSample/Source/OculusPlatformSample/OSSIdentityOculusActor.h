// Copyright 2016 Oculus VR, LLC All Rights reserved.

#pragma once

#include "OSSOculusActor.h"
#include "OSSIdentityOculusActor.generated.h"

/**
 * Simple example against the OnlineSubsystem Identity Interface
 */
UCLASS()
class OCULUSPLATFORMSAMPLE_API AOSSIdentityOculusActor : public AOSSOculusActor
{
	GENERATED_BODY()

private:
	FString PlayerName;
	FString PlayerID;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = Identity)
	void OnPlayerNameUpdate(const FString& PlayerName);

	UFUNCTION(BlueprintImplementableEvent, Category = Identity)
		void OnPlayerIDUpdate(const FString& PlayerID);



	virtual void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) override;

	void OnPrivilegeCheck(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 CheckResult);
};
