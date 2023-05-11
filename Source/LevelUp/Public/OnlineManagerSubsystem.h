// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineManagerSubsystem.generated.h"

class IOnlineSession;
class FOnlineSessionSearch;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionsFound);

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

	int32 Index = -1;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;
	FString HostUserName;
};

UCLASS()
class LEVELUP_API UOnlineManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	bool Host(const FString& ServerName);
	bool ResetSession();
	bool FindSession();
	bool Join(int32 Index);

	void GetListOfValidServers(TArray<FServerData>& Out) const;

	FOnSessionsFound OnSessionsFound;

protected:
	void OnCreateSessionComplite(FName SessionName, bool Success);
	void OnDestroySessionComplite(FName SessionName, bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type ResultType);
	void OnEndSessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);

	bool IsSessionDataValid() const;
	bool IsPrevSessionStillExist() const;

private:
	TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
