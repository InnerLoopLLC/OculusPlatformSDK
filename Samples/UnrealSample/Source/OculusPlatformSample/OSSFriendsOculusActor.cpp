// Copyright 2016 Oculus VR, LLC All Rights reserved.

#include "OculusPlatformSample.h"
#include "OSSFriendsOculusActor.h"

void AOSSFriendsOculusActor::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	auto OculusIdentityInterface = Online::GetIdentityInterface();
	OculusIdentityInterface->ClearOnLoginCompleteDelegate_Handle(0, OnLoginCompleteDelegateHandle);

	UE_LOG_ONLINE(Display, TEXT("Finished Login"));

	auto OculusFriendsInterface = Online::GetFriendsInterface();
	UE_LOG_ONLINE(Display, TEXT("Trying to get friends list from server"));

	OculusFriendsInterface->ReadFriendsList(
		0,
		TEXT("default"),
		FOnReadFriendsListComplete::CreateUObject(this, &AOSSFriendsOculusActor::OnReadFriendsListComplete)
		);
}

void AOSSFriendsOculusActor::OnReadFriendsListComplete(int32 LocalUserNum, const bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UE_LOG_ONLINE(Display, TEXT("Got results back from reading friends list"));
	if (bWasSuccessful)
	{
		auto OculusFriendsInterface = Online::GetFriendsInterface();
		TArray<TSharedRef<FOnlineFriend>> Friends;
		OculusFriendsInterface->GetFriendsList(0, ListName, Friends);
		UE_LOG_ONLINE(Display, TEXT("Friends list loaded.  Count of friends: %d"), Friends.Num());
		OnFriendCountUpdate(Friends.Num());
		int32 i = 0;
		for (auto Friend : Friends)
		{
			AddFriendToLevel(Friend->GetDisplayName(), i);
			i++;
		}

		if (Friends.Num() > 0)
		{
			auto FriendId = Friends[0]->GetUserId();
			auto Friend = OculusFriendsInterface->GetFriend(0, FriendId.Get(), ListName);
			UE_LOG_ONLINE(Display, TEXT("Got friend by id: %s"), *Friend->GetDisplayName());

			auto IsFriends = OculusFriendsInterface->IsFriend(0, FriendId.Get(), ListName);
			if (IsFriends)
			{
				UE_LOG_ONLINE(Display, TEXT("You are friends"));
			}
		}

		TArray<TSharedRef<FOnlineFriend>> OnlineFriends;
		OculusFriendsInterface->GetFriendsList(0, TEXT("onlinePlayers"), OnlineFriends);
		UE_LOG_ONLINE(Display, TEXT("Online Friends list loaded.  Count of friends: %d"), OnlineFriends.Num());
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Friends list could not be loaded"));
	}
}
