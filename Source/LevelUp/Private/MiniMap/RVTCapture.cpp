// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap/RVTCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VT/RuntimeVirtualTextureVolume.h"
#include "LevelUpLibrary.h"

// Sets default values
ARVTCapture::ARVTCapture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	const FSoftObjectPath BasicPlane("/Engine/BasicShapes/Plane", "Plane", {});
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
	Plane->SetupAttachment(Root);
	Plane->SetStaticMesh(LevelUp::FindOrLoadObject<UStaticMesh>(BasicPlane.ToString()));
	Plane->SetRelativeScale3D(FVector(5.12f, 5.12f, 1.0f));
	Plane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Plane->SetGenerateOverlapEvents(false);
	Plane->SetCanEverAffectNavigation(false);

	Capture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	Capture->SetupAttachment(Root);
	Capture->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	Capture->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Capture->ProjectionType = ECameraProjectionMode::Orthographic;
	Capture->OrthoWidth = 512;
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Atmosphere, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Cloud, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_BSP, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Decals, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Fog, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Landscape, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Particles, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_SkeletalMeshes, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Translucency, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Lighting, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_DeferredLighting, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_NaniteMeshes, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_InstancedStaticMeshes, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_InstancedFoliage, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_InstancedGrass, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Paper2DSprites, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_TextRender, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_AmbientOcclusion, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_DynamicShadows, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_SkyLighting, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_VolumetricFog, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_AmbientCubemap, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_DistanceFieldAO, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_LightFunctions, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_LightShafts, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_PostProcessing, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_ReflectionEnvironment, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_ScreenSpaceReflections, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_TexturedLightProfiles, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_AntiAliasing, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_TemporalAA, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_MotionBlur, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Bloom, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_Game, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_LocalExposure, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_EyeAdaptation, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_ToneCurve, false);
	Capture->ShowFlags.SetSingleFlag(FEngineShowFlags::EShowFlag::SF_PathTracing, false);

	Size = 1.0f;
}

void ARVTCapture::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARVTCapture::OnPositionUpdated, 0.05f, true);

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (UMaterialInterface* Mat = Plane->GetMaterial(0))
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Mat, this);
		Plane->SetMaterial(0, DynamicMaterial);
	}
	TArray<AActor*> FoundActors;
	if (const AActor* Volume = UGameplayStatics::GetActorOfClass(GetWorld(), ARuntimeVirtualTextureVolume::StaticClass()))
	{
		Size = Volume->GetActorRelativeScale3D().X;
	}
}

void ARVTCapture::OnPositionUpdated()
{
	if (!IsValid(DynamicMaterial) || !IsValid(PlayerController))
	{
		return;
	}
	if (AActor* Target = PlayerController->GetPawn())
	{
		FVector Offset;
		Offset = Target->GetActorLocation() / Size;
		FVector Forward = Target->GetActorForwardVector() * FVector(1.0f, 1.0f, 0.0f);
		Forward.Normalize();
		Offset.Z = (FMath::Acos(Forward.Dot(FVector(1.0f, 0.0f, 0.0f))) / (PI * 2.0f)) * FMath::Sign(Forward.Dot(FVector(0.0f, 1.0f, 0.0f)));
		DynamicMaterial->SetVectorParameterValue(TEXT("Offset"), Offset);
	}
}
