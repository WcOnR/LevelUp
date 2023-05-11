#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineManagerSubsystem.h"
#include "StartupMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;
class UEditableText;
class UListView;

UCLASS(BlueprintType)
class UServerDataObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FServerData Data;
	UPROPERTY(BlueprintReadWrite)
	bool Selected = false;
};

UCLASS(Abstract)
class UStartupMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void OnHostButton();
	UFUNCTION()
	void OnFindButton();
	UFUNCTION()
	void OnQuitButton();
	
	UFUNCTION()
	void OnCreateButton();

	UFUNCTION()
	void OnRefreshButton();
	UFUNCTION()
	void OnConnectButton();

	UFUNCTION()
	void OnBackButton();

	UFUNCTION()
	void OnListUpdated();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* FindButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UEditableText* ServerNameEditableText;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher* ServerListSwitcher;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UListView* ServerListView;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RefreshButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ConnectButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidget* MainLayout;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidget* CreateServerLayout;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidget* FindServerLayout;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWidget* LoadingLayout;

	UPROPERTY(BlueprintReadOnly, Category = HUD)
	TObjectPtr<APlayerController> PlayerOwner;

	UPROPERTY()
	UOnlineManagerSubsystem* OMSubsystem;
};