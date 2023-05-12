// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"

#include "LoadImage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOutputPin, UTexture2D*, Image);

UCLASS()
class LEVELUP_API ULoadImage : public UBlueprintAsyncActionBase
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FOutputPin OnFailure;

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "URL")
	static ULoadImage* LoadImage(UObject* WorldContextObject, const FString& URL);

	virtual void Activate() override;

	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	UObject* WorldContextObject;
	FString URL;
	FHttpRequestPtr HttpRequest;
};