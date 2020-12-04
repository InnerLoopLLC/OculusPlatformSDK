// Copyright 2016 Oculus VR, LLC All Rights reserved.

#pragma once

#include "OSSOculusActor.h"
#include "OSSFriendsOculusActor.generated.h"

/**
 *
 */
UCLASS()
class OCULUSPLATFORMSAMPLE_API AOSSFriendsOculusActor : public AOSSOculusActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = Friend)
	void OnFriendCountUpdate(int32 FriendCount);

	UFUNCTION(BlueprintImplementableEvent, Category = Friend)
	void AddFriendToLevel(const FString& FriendName, int32 Index);

	virtual void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) override;
	
	void OnReadFriendsListComplete(int32 LocalUserNum, const bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);
};
