// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineManagerSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/PlayerController.h"

DECLARE_LOG_CATEGORY_EXTERN(LogOnlineSubsystem, Log, All);
DEFINE_LOG_CATEGORY(LogOnlineSubsystem);

const static FName SESSION_NAME = TEXT("9d4e33c02ac09afcc26150107eb3ced115a8891c8c84ac736f5862b8f28fb975");
const static FName SETTING_KEY_SERVER_NAME = TEXT("LEVELUP_SERVER_NAME");

const static FString GAMEPLAY_MAP_NAME = TEXT("/Game/FirstPerson/Maps/FirstPersonMap");
const static FString STARTUP_MAP_NAME = TEXT("/Game/FirstPerson/Maps/StartupMap");

const static int32 NumPublicConnections = 3;
const static int32 MaxSearchResults = 100;
const static int32 PlayerId = 0;

void UOnlineManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	checkf(Subsystem, TEXT("IOnlineSubsystem::Get failed."));
	UE_LOG(LogOnlineSubsystem, Log, TEXT("Online subsystem found: %s"), *Subsystem->GetSubsystemName().ToString());
	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnCreateSessionComplite);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnCreateSessionComplite);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnJoinSessionComplete);
		SessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnEndSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineManagerSubsystem::OnFindSessionsComplete);
		if (SessionInterface->GetNamedSession(SESSION_NAME))
		{
			UE_LOG(LogOnlineSubsystem, Log, TEXT("Start destroying prev session..."));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		SessionSearch = MakeShared<FOnlineSessionSearch>();
	}
}

bool UOnlineManagerSubsystem::Host(const FString& ServerName)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	check(Subsystem && SessionInterface.IsValid());
	if (!ServerName.Len() || IsPrevSessionStillExist())
		return false;

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = false;
	if (Subsystem->GetSubsystemName() == TEXT("NULL"))
	{
		SessionSettings.bIsLANMatch = true;
	}
	SessionSettings.NumPublicConnections = NumPublicConnections;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.Set(SETTING_KEY_SERVER_NAME, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	UE_LOG(LogOnlineSubsystem, Log, TEXT("Start creating session."));
	SessionInterface->CreateSession(PlayerId, SESSION_NAME, SessionSettings);
	return true;
}

bool UOnlineManagerSubsystem::ResetSession()
{
	check(SessionInterface.IsValid());
	if (SessionInterface->GetNamedSession(SESSION_NAME))
	{
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Start destroying prev session..."));
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else
	{
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Reset session complite."));
		return false;
	}
	return true;
}

bool UOnlineManagerSubsystem::FindSession()
{
	check(SessionInterface.IsValid());
	if (IsPrevSessionStillExist())
		return false;

	SessionSearch->MaxSearchResults = MaxSearchResults;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogOnlineSubsystem, Log, TEXT("Start find session..."));
	SessionInterface->FindSessions(PlayerId, SessionSearch.ToSharedRef());
	return true;
}

bool UOnlineManagerSubsystem::Join(int32 Index)
{
	check(SessionInterface.IsValid());
	if (IsPrevSessionStillExist())
		return false;

	if (SessionSearch->SearchResults.Num() > Index && Index >= 0)
	{
		FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[Index];
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Join Session %s"), *Result.GetSessionIdStr());
		SessionInterface->JoinSession(PlayerId, SESSION_NAME, Result);
	}
	else
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("Trying to join %d id, but is SearchResults.Num() == %d"), Index, SessionSearch->SearchResults.Num());
		return false;
	}
	return true;
}

void UOnlineManagerSubsystem::GetListOfValidServers(TArray<FServerData>& Out) const
{
	Out.Empty();
	if (!SessionSearch.IsValid())
	{
		return;
	}
	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
	{
		const auto& Result = SessionSearch->SearchResults[i];
		FServerData Data;
		Data.Index = i;
		Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Data.HostUserName = Result.Session.OwningUserName;
		if (Result.Session.SessionSettings.Get<FString>(SETTING_KEY_SERVER_NAME, Data.Name))
		{
			Out.Add(Data);
		}
	}
}

void UOnlineManagerSubsystem::OnCreateSessionComplite(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("Failed to create session."));
		return;
	}
	UE_LOG(LogOnlineSubsystem, Log, TEXT("Session created."));

	UWorld* World = GetWorld();
	checkf(World, TEXT("OnCreateSessionComplite world doesn't exist!"));
	World->ServerTravel(FString::Printf(TEXT("%s?listen"), *GAMEPLAY_MAP_NAME));
}

void UOnlineManagerSubsystem::OnDestroySessionComplite(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Session %s destroyed"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("Failed to destroy session %s"), *SessionName.ToString());
	}
}

void UOnlineManagerSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType)
{
	if (ResultType == EOnJoinSessionCompleteResult::Success)
	{
		check(SessionInterface.IsValid() && GetGameInstance());
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Joined to session."));
		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		checkf(PlayerController, TEXT("OnJoinSessionComplete PlayerController doesn't exist!"));
		FString ConnectInfo;
		SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo);
		UE_LOG(LogOnlineSubsystem, Log, TEXT("ConnectInfo[%s] => %s"), *SessionName.ToString(), *ConnectInfo);
		PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("Failed to join session %d"), static_cast<int32>(ResultType));
	}
}

void UOnlineManagerSubsystem::OnEndSessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		UE_LOG(LogOnlineSubsystem, Log, TEXT("Session leaved."));
	}
	else
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("Failed to end session."));
	}
}

void UOnlineManagerSubsystem::OnFindSessionsComplete(bool Success)
{
	check(SessionSearch.IsValid());
	if (!Success)
	{
		UE_LOG(LogOnlineSubsystem, Error, TEXT("Failed to find session."));
		return;
	}
	UE_LOG(LogOnlineSubsystem, Log, TEXT("Find session complete [%d]"), SessionSearch->SearchResults.Num());
	for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
	{
		const auto& Result = SessionSearch->SearchResults[i];
		FServerData Data;
		Data.Index = i;
		if (!Result.Session.SessionSettings.Get<FString>(SETTING_KEY_SERVER_NAME, Data.Name))
		{
			Data.Name = Result.GetSessionIdStr();
		}
		Data.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Data.HostUserName = Result.Session.OwningUserName;
		UE_LOG(LogOnlineSubsystem, Log, TEXT("->[%d] %s - %s - %d/%d"), i, *Data.Name, *Data.HostUserName, Data.CurrentPlayers, Data.MaxPlayers);
	}
	OnSessionsFound.Broadcast();
}

bool UOnlineManagerSubsystem::IsSessionDataValid() const
{
	return SessionInterface.IsValid() && SessionSearch.IsValid();
}

bool UOnlineManagerSubsystem::IsPrevSessionStillExist() const
{
	if (SessionInterface->GetNamedSession(SESSION_NAME))
	{
		UE_LOG(LogOnlineSubsystem, Warning, TEXT("Prev session still exist. Please wait!"));
		return true;
	}
	return false;
}