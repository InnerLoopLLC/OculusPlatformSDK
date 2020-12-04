// Copyright 2016 Oculus VR, LLC All Rights reserved.

#pragma once

#include "OSSOculusActor.h"
#include "OSSAchievementOculusActor.generated.h"

/**
 *
 */
UCLASS()
class OCULUSPLATFORMSAMPLE_API AOSSAchievementOculusActor : public AOSSOculusActor
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = Achievement)
		void OnAchievementsUpdated(const FString& AchStatus);

	virtual void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error) override;

	void OnQueryAchievementDescriptionsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

	void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

	void OnAchievementsWritten(const FUniqueNetId& PlayerId, const bool bWasSuccessful);
	
};
