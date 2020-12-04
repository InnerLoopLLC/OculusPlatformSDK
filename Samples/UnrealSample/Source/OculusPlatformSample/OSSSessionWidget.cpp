// Copyright 2016 Oculus VR, LLC All Rights reserved.

#include "OculusPlatformSample.h"
#include "OnlineSessionSettings.h"
#include "OSSSessionWidget.h"

void UOSSSessionWidget::ReadOnlineFriends()
{
	auto OculusFriendsInterface = Online::GetFriendsInterface();

	UE_LOG_ONLINE(Display, TEXT("Trying to get friends list from server"));

	OculusFriendsInterface->ReadFriendsList(
		0,
		TEXT("onlinePlayers"),
		FOnReadFriendsListComplete::CreateUObject(this, &UOSSSessionWidget::OnReadFriendsListComplete)
		);
}

void UOSSSessionWidget::OnReadFriendsListComplete(int32 LocalUserNum, const bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UE_LOG_ONLINE(Display, TEXT("Got results back from reading friends list"));
	if (bWasSuccessful)
	{
		auto OculusFriendsInterface = Online::GetFriendsInterface();
		TArray<TSharedRef<FOnlineFriend>> Friends;
		OculusFriendsInterface->GetFriendsList(0, ListName, Friends);
		UE_LOG_ONLINE(Display, TEXT("Online Friends list loaded.  Count of friends: %d"), Friends.Num());

		auto OculusSessionInterface = Online::GetSessionInterface();

		if (!OnFindFriendSessionCompleteDelegate.IsBound())
		{
			OnFindFriendSessionCompleteDelegate = FOnFindFriendSessionCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnFindFriendSessionComplete);
			OculusSessionInterface->AddOnFindFriendSessionCompleteDelegate_Handle(0, OnFindFriendSessionCompleteDelegate);
		}

		for (auto Friend : Friends)
		{
			OculusSessionInterface->FindFriendSession(0, Friend->GetUserId().Get());
			FriendsToInvite.Add(Friend->GetDisplayName(), Friend->GetUserId());
			AddFriendToInviteList(Friend->GetDisplayName());
		}
	}
	else
	{
		UE_LOG_ONLINE(Error, TEXT("%s"), *ErrorStr);
	}
}

void UOSSSessionWidget::OnFindFriendSessionComplete(int32 LocalUserNum, const bool bWasSuccessful, const FOnlineSessionSearchResult& FriendSearchResult)
{
	UE_LOG_ONLINE(Display, TEXT("Got result if friend is in a session"));
	if (bWasSuccessful)
	{
		if (FriendSearchResult.IsValid())
		{
			UE_LOG_ONLINE(Display, TEXT("Search result is valid.  Adding to the list"));
			AddFriendToList(FriendSearchResult.Session.OwningUserName);
			FriendsSessions.Add(FriendSearchResult.Session.OwningUserName, FriendSearchResult);
			return;
		}
	}
}

void UOSSSessionWidget::InviteFriendToSession(const FString& FriendName)
{
	UE_LOG_ONLINE(Display, TEXT("Trying to invite %s"), *FriendName);
	if (FriendsToInvite.Contains(FriendName))
	{
		auto OculusSessionInterface = Online::GetSessionInterface();
		OculusSessionInterface->SendSessionInviteToFriend(0, TEXT("Game"), FriendsToInvite[FriendName].Get());
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Could not find %s"), *FriendName);
	}
}

void UOSSSessionWidget::JoinFriendSession(const FString& FriendName)
{
	UE_LOG_ONLINE(Display, TEXT("Trying to join %s's session"), *FriendName);
	if (FriendsSessions.Contains(FriendName))
	{
		auto OculusSessionInterface = Online::GetSessionInterface();
		OculusSessionInterface->JoinSession(0, TEXT("Game"), FriendsSessions[FriendName]);
	}
	else
	{
		UE_LOG_ONLINE(Warning, TEXT("Could not find %s"), *FriendName);
	}
}

void UOSSSessionWidget::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	auto OculusSessionInterface = Online::GetSessionInterface();
	auto Session = OculusSessionInterface->GetNamedSession(SessionName);
	if (Session)
	{
		UE_LOG_ONLINE(Display, TEXT("Got back %s's session"), *Session->OwningUserName);
	}
	OnJoinSessionComplete(SessionName, JoinResult == EOnJoinSessionCompleteResult::Success);
}

void UOSSSessionWidget::CreateSession(const FName SessionName)
{
	auto OculusSessionInterface = Online::GetSessionInterface();

	if (!OculusSessionInterface.IsValid())
	{
		return;
	}

	UE_LOG_ONLINE(Display, TEXT("Trying to create a session"));

	if (!OnCreateSessionCompleteDelegate.IsBound())
	{
		OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnCreateSessionComplete);
		OculusSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	}

	TSharedPtr<class FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->NumPublicConnections = 2;
	SessionSettings->bShouldAdvertise = true;

	OculusSessionInterface->CreateSession(/* Hosting Player Num*/ 0, TEXT("Game"), *SessionSettings);
}

void UOSSSessionWidget::StartSession(FName SessionName)
{

	UE_LOG_ONLINE(Display, TEXT("Start Session"));

	auto OculusSessionInterface = Online::GetSessionInterface();

	auto Session = OculusSessionInterface->GetNamedSession(SessionName);

	UE_LOG_ONLINE(Display, TEXT("Session owned by %s"), *Session->OwningUserName);
	UE_LOG_ONLINE(Display, TEXT("Session state: %s"), EOnlineSessionState::ToString(Session->SessionState));

	if (!OnStartSessionCompleteDelegate.IsBound())
	{
		OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnStartSessionComplete);
		OculusSessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	}

	OculusSessionInterface->StartSession(SessionName);
}

int32 UOSSSessionWidget::RefreshPlayerCountInSession()
{
	auto OculusSessionInterface = Online::GetSessionInterface();

	if (!OculusSessionInterface.IsValid())
	{
		return 0;
	}

	auto Session = OculusSessionInterface->GetNamedSession(TEXT("Game"));

	if (!Session)
	{
		return 0;
	}

	return Session->RegisteredPlayers.Num();
}

void UOSSSessionWidget::EndSession(FName SessionName)
{

	UE_LOG_ONLINE(Display, TEXT("End Session"));

	auto OculusSessionInterface = Online::GetSessionInterface();

	auto Session = OculusSessionInterface->GetNamedSession(SessionName);

	UE_LOG_ONLINE(Display, TEXT("Session owned by %s"), *Session->OwningUserName);
	UE_LOG_ONLINE(Display, TEXT("Session state: %s"), EOnlineSessionState::ToString(Session->SessionState));

	if (!OnEndSessionCompleteDelegate.IsBound())
	{
		OnEndSessionCompleteDelegate = FOnEndSessionCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnEndSessionComplete);
		OculusSessionInterface->AddOnEndSessionCompleteDelegate_Handle(OnEndSessionCompleteDelegate);
	}

	OculusSessionInterface->EndSession(SessionName);
}

void UOSSSessionWidget::DestroySession(FName SessionName)
{

	UE_LOG_ONLINE(Display, TEXT("Destroy Session"));

	auto OculusSessionInterface = Online::GetSessionInterface();
	auto Session = OculusSessionInterface->GetNamedSession(SessionName);

	UE_LOG_ONLINE(Display, TEXT("Session owned by %s"), *Session->OwningUserName);
	UE_LOG_ONLINE(Display, TEXT("Session state: %s"), EOnlineSessionState::ToString(Session->SessionState));

	if (!OnDestroySessionCompleteDelegate.IsBound())
	{
		OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnDestroySessionComplete);
		OculusSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
	}

	OculusSessionInterface->DestroySession(SessionName);
}

void UOSSSessionWidget::OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult & InviteResult)
{
	if (!bWasSuccessful)
	{
		UE_LOG_ONLINE(Error, TEXT("Did not successfully invited user to the session!"));
		return;
	}

	UE_LOG_ONLINE(Display, TEXT("Accepted invite to session.  Joining session...."));

	auto OculusSessionInterface = Online::GetSessionInterface();
	OculusSessionInterface->JoinSession(ControllerId, TEXT("Game"), InviteResult);
}

void UOSSSessionWidget::StartMatchmaking(const FString& PoolName)
{
	auto OculusSessionInterface = Online::GetSessionInterface();

	TArray< TSharedRef<const FUniqueNetId> > LocalPlayers;

	// Create a matchmaking for two people
	auto SessionSettings = new FOnlineSessionSettings();
	SessionSettings->NumPublicConnections = 2;

	SearchSettings = MakeShareable(new FOnlineSessionSearch());

	// Add the delegate
	if (!OnMatchmakingCompleteDelegate.IsBound())
	{
		OnMatchmakingCompleteDelegate = FOnMatchmakingCompleteDelegate::CreateUObject(this, &UOSSSessionWidget::OnMatchmakingComplete);
		OculusSessionInterface->AddOnMatchmakingCompleteDelegate_Handle(OnMatchmakingCompleteDelegate);
	}

	// Search with this poolname
	SearchSettings->QuerySettings.Set(SETTING_MAPNAME, PoolName, EOnlineComparisonOp::Equals);

	TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSettings.ToSharedRef();

	// Do the search
	OculusSessionInterface->StartMatchmaking(LocalPlayers, TEXT("Game"), *SessionSettings, SearchSettingsRef);
}

void UOSSSessionWidget::OnMatchmakingComplete(FName SessionName, bool bWasSuccessful)
{
	if (!(bWasSuccessful && SearchSettings->SearchResults.Num() > 0))
	{
		UE_LOG_ONLINE(Error, TEXT("Did not successfully find a matchmaking session!"));
		return;
	}

	UE_LOG_ONLINE(Display, TEXT("Found a matchmaking session.  Joining session...."));

	auto OculusSessionInterface = Online::GetSessionInterface();
	OculusSessionInterface->JoinSession(0, SessionName, SearchSettings->SearchResults[0]);
}
