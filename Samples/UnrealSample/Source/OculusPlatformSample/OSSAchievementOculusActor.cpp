// Copyright 2016 Oculus VR, LLC All Rights reserved.

#include "OculusPlatformSample.h"
#include "OSSAchievementOculusActor.h"

void AOSSAchievementOculusActor::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	auto OculusIdentityInterface = Online::GetIdentityInterface();
	OculusIdentityInterface->ClearOnLoginCompleteDelegate_Handle(0, OnLoginCompleteDelegateHandle);

	UE_LOG_ONLINE(Display, TEXT("Finished Login: %s"), *UserId.ToString());
	auto OculusAchievementInterface = Online::GetAchievementsInterface();

	UE_LOG_ONLINE(Display, TEXT("Trying to load achievement definitions from server...."));
	OculusAchievementInterface->QueryAchievementDescriptions(
		UserId,
		FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &AOSSAchievementOculusActor::OnQueryAchievementDescriptionsComplete)
		);
}

void AOSSAchievementOculusActor::OnQueryAchievementDescriptionsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	UE_LOG_ONLINE(Display, TEXT("Got results back from achievement definitions"));

	if (bWasSuccessful)
	{
		UE_LOG_ONLINE(Display, TEXT("Achievement descriptions loaded for: %s"), *PlayerId.ToString());
		auto OculusAchievementInterface = Online::GetAchievementsInterface();

		// The achievement I defined for my app
		FString AchievementId(TEXT("SimpleAchievement"));

		FOnlineAchievementDesc AchievementDesc;
		UE_LOG_ONLINE(Display, TEXT("Try to get cached achievement desc: %s"), *AchievementId);
		if (OculusAchievementInterface->GetCachedAchievementDescription(AchievementId, AchievementDesc) == EOnlineCachedResult::NotFound)
		{
			UE_LOG_ONLINE(Error, TEXT("Could not find achievement id: %s.  Exiting early!"), *AchievementId);
			return;
		}

		UE_LOG_ONLINE(Display, TEXT("Got Achievement: %s"), *AchievementDesc.Title.ToString());

		UE_LOG_ONLINE(Display, TEXT("Trying to load player achievements from server...."));
		OculusAchievementInterface->QueryAchievements(
			PlayerId,
			FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &AOSSAchievementOculusActor::OnQueryAchievementsComplete)
			);
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Achievement descriptions could not be loaded!"));
	}
}

void AOSSAchievementOculusActor::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	UE_LOG_ONLINE(Display, TEXT("Got results back for player achievements"));

	if (bWasSuccessful)
	{
		UE_LOG_ONLINE(Display, TEXT("PlayerAchievements loaded"));
		auto OculusAchievementInterface = Online::GetAchievementsInterface();
		
		// The simple achievement I defined for my app
		FString SimpleAchievementId(TEXT("SimpleAchievement"));

		FOnlineAchievement SimpleAchievement;
		UE_LOG_ONLINE(Display, TEXT("Try to get cached simple achievement: %s"), *SimpleAchievementId);
		OculusAchievementInterface->GetCachedAchievement(PlayerId, SimpleAchievementId, SimpleAchievement);

		UE_LOG_ONLINE(Display, TEXT("Got Simple Achievement: %s, Progress: %f"), *SimpleAchievement.Id, SimpleAchievement.Progress);

		// The count achievement I defined for my app
		FString CountAchievementId(TEXT("LongProgressAchievement"));

		FOnlineAchievement CountAchievement;
		UE_LOG_ONLINE(Display, TEXT("Try to get cached count achievement: %s"), *CountAchievementId);
		OculusAchievementInterface->GetCachedAchievement(PlayerId, CountAchievementId, CountAchievement);

		UE_LOG_ONLINE(Display, TEXT("Got Count Achievement: %s, Progress: %f"), *CountAchievement.Id, CountAchievement.Progress);

		// The bitfield achievement I defined for my app
		FString BitFieldAchievementId(TEXT("LotsOfBitFields"));

		FOnlineAchievement BitfieldAchievement;
		UE_LOG_ONLINE(Display, TEXT("Try to get cached bitfield achievement: %s"), *BitFieldAchievementId);
		OculusAchievementInterface->GetCachedAchievement(PlayerId, BitFieldAchievementId, BitfieldAchievement);

		UE_LOG_ONLINE(Display, TEXT("Got Bitfield Achievement: %s, Progress: %f"), *BitfieldAchievement.Id, BitfieldAchievement.Progress);

		UE_LOG_ONLINE(Display, TEXT("Trying to update player achievements to server...."));
		FOnlineAchievementsWritePtr WriteObject = MakeShareable(new FOnlineAchievementsWrite());
		WriteObject->SetIntStat(*CountAchievementId, 1);
		WriteObject->SetIntStat(*BitFieldAchievementId, 10);
		FOnlineAchievementsWriteRef WriteObjectRef = WriteObject.ToSharedRef();
		OculusAchievementInterface->WriteAchievements(
			PlayerId,
			WriteObjectRef,
			FOnAchievementsWrittenDelegate::CreateUObject(this, &AOSSAchievementOculusActor::OnAchievementsWritten)
			);
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Achievements could not be loaded!"));
	}
}

void AOSSAchievementOculusActor::OnAchievementsWritten(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	UE_LOG_ONLINE(Display, TEXT("Got results back from achievement write"));

	if (bWasSuccessful)
	{
		UE_LOG_ONLINE(Display, TEXT("Achievements written successfully!"));
		OnAchievementsUpdated(TEXT("Achievements written successfully!"));
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Achievements could not be written!"));
		OnAchievementsUpdated(TEXT("Achievements could not be written!"));
	}
}
