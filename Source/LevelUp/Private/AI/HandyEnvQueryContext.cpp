// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HandyEnvQueryContext.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "AI/CoverVolume.h"

AActor* GetActorFromInstance(FEnvQueryInstance& QueryInstance)
{
	UObject* QuerierObject = QueryInstance.Owner.Get();
	if (QuerierObject == nullptr)
	{
		return nullptr;
	}
	return Cast<AActor>(QuerierObject);
}

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if (AActor* QuerierActor = GetActorFromInstance(QueryInstance))
	{
		AActor* ResultingActor = UGameplayStatics::GetPlayerPawn(QuerierActor->GetWorld(), 0);
		if (ResultingActor)
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, ResultingActor);
		}
	}
}

void UEnvQueryContext_CoverVolume::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if (AActor* QuerierActor = GetActorFromInstance(QueryInstance))
	{
		TArray<AActor*> ActorSet;
		UGameplayStatics::GetAllActorsOfClass(QuerierActor->GetWorld(), ACoverVolume::StaticClass(), ActorSet);
		ActorSet.Remove(nullptr);
		if (ActorSet.Num())
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, ActorSet);
		}
	}
}
