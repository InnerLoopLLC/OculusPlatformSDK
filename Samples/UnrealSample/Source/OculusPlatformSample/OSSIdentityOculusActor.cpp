// Copyright 2016 Oculus VR, LLC All Rights reserved.

#include "OculusPlatformSample.h"
#include "OSSIdentityOculusActor.h"

void AOSSIdentityOculusActor::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	auto OculusIdentityInterface = Online::GetIdentityInterface();
	OculusIdentityInterface->ClearOnLoginCompleteDelegate_Handle(0, OnLoginCompleteDelegateHandle);

	OculusIdentityInterface->GetUserPrivilege(
		UserId,
		EUserPrivileges::CanPlay,
		IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateUObject(this, &AOSSIdentityOculusActor::OnPrivilegeCheck));

	if (!bWasSuccessful)
	{
		UE_LOG_ONLINE(Warning, TEXT("Unable to login with oculus! %s"), *Error);
		return;
	}
	UE_LOG_ONLINE(Display, TEXT("Logged in successfully to oculus!"));

	// Get the Oculus ID
	PlayerName = OculusIdentityInterface->GetPlayerNickname(UserId);
	PlayerID = UserId.ToString();

	UE_LOG_ONLINE(Display, TEXT("Welcome %s!"), *PlayerName);
	OnPlayerNameUpdate(PlayerName);
	OnPlayerIDUpdate(PlayerID);
}

void AOSSIdentityOculusActor::OnPrivilegeCheck(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 CheckResult)
{
	if (CheckResult != (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{
		UE_LOG_ONLINE(Error, TEXT("Arrg, you failed the entitlement check!"));

		// Developers may want to just quit the game here.
		PlayerName = TEXT("FAILED ENTITLEMENT CHECK");
		OnPlayerNameUpdate(PlayerName);
		return;
	}
	UE_LOG_ONLINE(Display, TEXT("You passed the entitlement check!  Good job upstanding citizen!"));
}
