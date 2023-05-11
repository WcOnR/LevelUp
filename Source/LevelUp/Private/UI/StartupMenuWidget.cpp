#include "UI/StartupMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/ListView.h"

static const int32 SERVER_NAME_MAX_LENGTH = 128;

void UStartupMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(GetGameInstance());
	OMSubsystem = GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>();
	check(OMSubsystem);
	OMSubsystem->OnSessionsFound.AddDynamic(this, &UStartupMenuWidget::OnListUpdated);
	PlayerOwner = GetGameInstance()->GetFirstLocalPlayerController();

	check(PlayerOwner);
	PlayerOwner->bShowMouseCursor = true;
	PlayerOwner->SetInputMode(FInputModeUIOnly());

	HostButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnHostButton);
	FindButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnFindButton);
	QuitButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnQuitButton);

	CreateButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnCreateButton);

	RefreshButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnRefreshButton);
	ConnectButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnConnectButton);

	BackButton->OnClicked.AddDynamic(this, &UStartupMenuWidget::OnBackButton);
}

void UStartupMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	PlayerOwner->bShowMouseCursor = false;
	PlayerOwner->SetInputMode(FInputModeGameOnly());
}

void UStartupMenuWidget::OnHostButton()
{
	BackButton->SetVisibility(ESlateVisibility::Visible);
	MenuSwitcher->SetActiveWidget(CreateServerLayout);
}

void UStartupMenuWidget::OnFindButton()
{
	MenuSwitcher->SetActiveWidget(LoadingLayout);
	OMSubsystem->FindSession();
}

void UStartupMenuWidget::OnQuitButton()
{
	if (IsValid(PlayerOwner))
	{
		PlayerOwner->ConsoleCommand("quit");
	}
}

void UStartupMenuWidget::OnCreateButton()
{
	const FString& ServerName = ServerNameEditableText->GetText().ToString();
	if (!ServerName.IsEmpty())
	{
		OMSubsystem->Host(ServerName.Left(SERVER_NAME_MAX_LENGTH));
	}
}

void UStartupMenuWidget::OnRefreshButton()
{
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
	OnFindButton();
}

void UStartupMenuWidget::OnConnectButton()
{
	OMSubsystem->Join(ServerListView->GetIndexForItem(ServerListView->GetSelectedItem()));
}

void UStartupMenuWidget::OnBackButton()
{
	MenuSwitcher->SetActiveWidget(MainLayout);
	BackButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UStartupMenuWidget::OnListUpdated()
{
	BackButton->SetVisibility(ESlateVisibility::Visible);
	MenuSwitcher->SetActiveWidget(FindServerLayout);
	ServerListView->ClearListItems();

	TArray<FServerData> ServerDatas;
	OMSubsystem->GetListOfValidServers(ServerDatas);

	ServerListSwitcher->SetActiveWidgetIndex(ServerDatas.Num() > 0 ? 0 : 1);
	ConnectButton->SetIsEnabled(ServerDatas.Num() > 0);
	for (const FServerData& Data : ServerDatas)
	{
		UServerDataObject* DataObject = NewObject<UServerDataObject>(UServerDataObject::StaticClass());
		DataObject->Data = Data;
		ServerListView->AddItem(DataObject);
	}
}