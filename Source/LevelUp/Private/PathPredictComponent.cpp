// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathPredictComponent.h"
#include "Components/SplineMeshComponent.h"

static const int32 PoolLimit = 50;

UPathPredictComponent::UPathPredictComponent()
{
}

void UPathPredictComponent::DrawPath(const FPredictProjectilePathResult& Path)
{
	ClearPath();
	for (int32 i = 0; i < Path.PathData.Num(); ++i)
	{
		AddSplinePoint(Path.PathData[i].Location, ESplineCoordinateSpace::World, false);
	}
	for (int32 i = 0; i < PoolLimit; ++i)
	{
		if (i < (Path.PathData.Num() - 1))
		{
			FVector Location, Tangent, LocationNext, TangentNext;
			GetLocalLocationAndTangentAtSplinePoint(i, Location, Tangent);
			GetLocalLocationAndTangentAtSplinePoint(i + 1, LocationNext, TangentNext);
			SplineMeshes[i]->SetStartAndEnd(Location, LocationNext, LocationNext, LocationNext);
			SplineMeshes[i]->SetVisibility(true);
		}
	}
	UpdateSpline();
}

void UPathPredictComponent::ClearPath()
{
	ClearSplinePoints();
	for (USplineMeshComponent* SplineMesh : SplineMeshes)
	{
		SplineMesh->SetVisibility(false);
	}
}

void UPathPredictComponent::BeginPlay()
{
	Super::BeginPlay();
	ClearSplinePoints();
	for (int32 i = 0; i < PoolLimit; ++i)
	{
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComp->SetMobility(EComponentMobility::Movable);
		SplineMeshComp->RegisterComponent();
		SplineMeshComp->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
		GetOwner()->AddInstanceComponent(SplineMeshComp);
		SplineMeshComp->SetStaticMesh(Mesh);
		SplineMeshComp->SetVisibility(false);
		SplineMeshes.Add(SplineMeshComp);
	}
}
