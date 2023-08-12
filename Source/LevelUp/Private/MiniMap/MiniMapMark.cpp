// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap/MiniMapMark.h"
#include "MiniMap/Follower.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "LevelUpLibrary.h"

// Sets default values
AMiniMapMark::AMiniMapMark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	const FSoftObjectPath BasicPlane("/Engine/BasicShapes/Plane", "Plane", {});
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(Root);
	Plane->SetStaticMesh(LevelUp::FindOrLoadObject<UStaticMesh>(BasicPlane.ToString()));
	Plane->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	Plane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Plane->SetGenerateOverlapEvents(false);
	Plane->SetCanEverAffectNavigation(false);
	Plane->VirtualTextureRenderPassType = ERuntimeVirtualTextureMainPassType::Never;
	Follower = CreateDefaultSubobject<UFollower>(TEXT("Follower"));
}

void AMiniMapMark::BeginPlay()
{
	Super::BeginPlay();
	if (UMaterialInterface* Mat = Plane->GetMaterial(0))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Mat, this);
		DynamicMaterial->SetVectorParameterValue(TEXT("MainColor"), MainColor);
		Plane->SetMaterial(0, DynamicMaterial);
	}
	GetOwner()->OnDestroyed.AddDynamic(this, &AMiniMapMark::OnOwnerDestroyed);
}

void AMiniMapMark::OnOwnerDestroyed(AActor* DestroyedActor)
{
	Destroy();
}
