// Copyright Epic Games, Inc. All Rights Reserved.
#include "LoadImage.h"
#include "HTTP.h"
#include "ImageUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogLoadImage, Log, All);

ULoadImage::ULoadImage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), WorldContextObject(nullptr)
{
}

ULoadImage* ULoadImage::LoadImage(UObject* WorldContextObject, const FString& URL)
{
	ULoadImage* BlueprintNode = NewObject<ULoadImage>();
	BlueprintNode->WorldContextObject = WorldContextObject;
	BlueprintNode->URL = URL;
	BlueprintNode->HttpRequest = FHttpModule::Get().CreateRequest();
	return BlueprintNode;
}

void ULoadImage::Activate()
{
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ULoadImage::OnRequestComplete);
	HttpRequest->SetURL(URL);
	HttpRequest->ProcessRequest();
}

void ULoadImage::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != EHttpResponseCodes::Ok)
	{
		OnFailure.Broadcast(nullptr);
		return;
	}
	const TArray<uint8>& Buffer = Response->GetContent();
	TArrayView64<const uint8> Buffer64(Buffer.GetData(), Buffer.Num());
	FImage Image;
	if (!FImageUtils::DecompressImage(Buffer64.GetData(), Buffer64.Num(), Image))
	{
		UE_LOG(LogLoadImage, Error, TEXT("Error creating texture. Couldn't determine the file format"));
		OnFailure.Broadcast(nullptr);
		return;
	}
	OnSuccess.Broadcast(FImageUtils::CreateTexture2DFromImage(Image));
}