// Copyright Epic Games, Inc. All Rights Reserved.

#include "LoadVolume.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ALoadVolume::ALoadVolume()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Trigger");

	// Set as root component
	RootComponent = CollisionComp;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(CollisionComp);
}

void ALoadVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
									  AActor* OtherActor,
									  UPrimitiveComponent* OtherComp,
									  int32 OtherBodyIndex,
									  bool bFromSweep,
									  const FHitResult& SweepResult)
{
	if (!IsValid(StaticMesh) && (OtherActor != nullptr) && (OtherActor == UGameplayStatics::GetPlayerPawn(this, 0)))
	{
		if (UStaticMesh* MeshData = Cast<UStaticMesh>(MeshPath.TryLoad()))
		{
			StaticMesh = MeshData;
			StaticMeshComponent->SetStaticMesh(StaticMesh);
		}
	}
}

void ALoadVolume::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ALoadVolume::OnBeginOverlap);
}
