// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/GrapplingHookSystem/BWGrabPoint.h"

#include <string>

#include "Features/Player/BWCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"


ABWGrabPoint::ABWGrabPoint()
{
	PrimaryActorTick.bCanEverTick = true;
	LandingPoint = FVector::ZeroVector;
	bShowDebug = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("BlockAllDynamic");
	constexpr ECollisionChannel GrabPointsChannel = ECC_GameTraceChannel1;
	constexpr ECollisionChannel PlayerChannel = ECC_GameTraceChannel3;
	Mesh->SetCollisionResponseToChannel(GrabPointsChannel, ECR_Block);
	Mesh->SetCollisionResponseToChannel(PlayerChannel, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

void ABWGrabPoint::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

FVector ABWGrabPoint::GetLocation() const
{
	return bInitialized ? GetActorLocation() : FVector::ZeroVector;
}

FVector ABWGrabPoint::GetLandingPoint() const
{
	return bInitialized ? GetActorTransform().TransformPosition(LandingPoint) : FVector::ZeroVector;
}

bool ABWGrabPoint::CanBeGrabbed(const ACharacter* Character) const
{
	if (Character == nullptr || !bInitialized)
	{
		return false;
	}

	// Check if the distance is not too close to the player
	const float SquaredDistance = FVector::DistSquared(GetActorLocation(), Character->GetActorLocation());
	if (SquaredDistance < PlayerGrapplingHook->GetMinDistance() * PlayerGrapplingHook->GetMinDistance())
	{
		return false;
	}

	// Check if there are any obstacles between the player and the grab point
	TArray<FHitResult> HitResults;
	if (ObstacleInBetween(Character, HitResults))
	{
		return false;
	}

	// Check how close the grab point is to the camera's forward vector
	const FVector Direction = (GetActorLocation() - GetPlayerCameraLocation()).GetSafeNormal();
	const FVector CameraForward = Character->GetControlRotation().Vector();
	const float ToleranceMultiplier = SquaredDistance / (PlayerGrapplingHook->GetMaxDistance() * PlayerGrapplingHook->
		GetMaxDistance());
	const float Tolerance = FMath::Lerp(0.999f, 0.8f, ToleranceMultiplier);
	const bool bCoincident = FVector::Coincident(Direction.GetSafeNormal(), CameraForward, Tolerance);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	if (bShowDebug && bCoincident)
		DrawDebugSphere(GetWorld(), GetActorLocation(), 100, 12, bCoincident ? FColor::Green : FColor::Red, false, -1,
		                0,
		                1);
#endif

	return bCoincident;
}

FVector ABWGrabPoint::GetPlayerCameraLocation() const
{
	return UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraCacheView().Location;
}

bool ABWGrabPoint::ObstacleInBetween(const ACharacter* Character, TArray<FHitResult>& HitResults) const
{
	// between landing point and player
	GetWorld()->LineTraceMultiByProfile(HitResults, GetLandingPoint(), Character->GetActorLocation(), "BlockAll",
	                                    TraceParams);
	if (HitResults.Num() > 0)
	{
		return true;
	}
	// between grab point and player
	GetWorld()->LineTraceMultiByProfile(HitResults, GetActorLocation(), Character->GetActorLocation(), "BlockAll",
	                                    TraceParams);
	if (HitResults.Num() > 0)
	{
		return true;
	}
	return false;
	
}

void ABWGrabPoint::Initialize()
{
	const ABWCharacter* PlayerCharacter = Cast<ABWCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter == nullptr)
	{
		bInitialized = false;
		return;
	}

	PlayerGrapplingHook = PlayerCharacter->GetGrapplingHook();
	if (!IsValid(PlayerGrapplingHook))
	{
		bInitialized = false;
		return;
	}

	Mesh->SetCollisionProfileName("BlockAllDynamic");
	constexpr ECollisionChannel GrabPointsChannel = ECC_GameTraceChannel1;
	constexpr ECollisionChannel PlayerChannel = ECC_GameTraceChannel3;
	Mesh->SetCollisionResponseToChannel(GrabPointsChannel, ECR_Block);
	Mesh->SetCollisionResponseToChannel(PlayerChannel, ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);

	TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	TraceParams.bTraceComplex = false;
	TraceParams.AddIgnoredActor(PlayerCharacter);
	TraceParams.AddIgnoredActor(this);

	bInitialized = true;
}
