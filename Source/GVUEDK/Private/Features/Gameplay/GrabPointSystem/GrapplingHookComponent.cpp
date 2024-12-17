// Copyright Villains, Inc. All Rights Reserved.


#include "GVUEDK/Public/Features/Gameplay/GrapplingHookSystem/GrapplingHookComponent.h"

#include <string>
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"


UGrapplingHookComponent::UGrapplingHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTargetAcquired = false;
	bShowDebug = false;
	bIsHooking = false;
	bOrientRotationToMovement = true;
	MaxDistance = 3000.f;
	MinDistance = 0.f;
	LinearSpeed = 2000.f;
	TotalHookDistance = 0.f;
	StartDelay = 0.f;
	ElapsedTime = 0.f;
	StartHookLocation = FVector::ZeroVector;
	EndHookLocation = FVector::ZeroVector;
	StartHookDirection = FVector::ZeroVector;
	SpeedCurve = nullptr;
	TargetGrabPoint = nullptr;
}

void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();
	InRangeGrabPoints = TSet<IGrabPoint*>();
}

void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FGvDebug::Log(TargetGrabPoint != nullptr ? Cast<AActor>(TargetGrabPoint)->GetName() : "No Target", true);
	//FGvDebug::Log(std::to_string(InRangeGrabPoints.Num()).c_str(), true);
	if (bIsHooking)
	{
		if (ElapsedTime < StartDelay)
		{
			ElapsedTime += DeltaTime;
			return;
		}
		PerformHooking(DeltaTime);
		return;
	}
	
	// check if there are any grab points in range (MaxCheckDistance)
	if (LookForGrabPoints(InRangeGrabPoints))
	{
		TargetGrabPoint = GetNearestGrabPoint(InRangeGrabPoints);
		bTargetAcquired = TargetGrabPoint != nullptr;
		return;
	}
	// if there are no grab points in range, reset the target
	if (bTargetAcquired)
	{
		TargetGrabPoint = nullptr;
		bTargetAcquired = false;
	}
}

void UGrapplingHookComponent::StartHooking()
{
	if (!bTargetAcquired || bIsHooking)
	{
		return;
	}
	ElapsedTime = 0.f;
	StartHookLocation = GetOwner()->GetActorLocation();
	EndHookLocation = TargetGrabPoint->GetLandingPoint();
	TotalHookDistance = FVector::Dist(StartHookLocation, EndHookLocation);
	StartHookDirection = (EndHookLocation - StartHookLocation).GetSafeNormal();
	OnStartHooking.Broadcast(StartHookLocation, EndHookLocation, StartHookDirection, TotalHookDistance);
	if (bOrientRotationToMovement)
		GetOwner()->SetActorRotation(FRotator(StartHookDirection.Rotation().Roll, StartHookDirection.Rotation().Yaw, 0.f));
	bIsHooking = true;
}

void UGrapplingHookComponent::StopHooking()
{
	TargetGrabPoint = nullptr;
	bTargetAcquired = false;
	bIsHooking = false;
	OnStopHooking.Broadcast();
}

void UGrapplingHookComponent::PerformHooking(float DeltaTime)
{
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + StartHookDirection * GetSpeed() * DeltaTime/*, false, nullptr, ETeleportType::TeleportPhysics*/);
	OnHooking.Broadcast();
	if (GetElapsedNormalizedDistance() >= 1)
	{
		GetOwner()->SetActorLocation(EndHookLocation);
		StopHooking();
	}
}

float UGrapplingHookComponent::GetSpeed() const
{
	if (!IsValid(SpeedCurve))
	{
		return LinearSpeed;
	}
	const float CurveInput = SpeedCurve->GetFloatValue(FVector::DistSquared(GetOwner()->GetActorLocation(), StartHookLocation) / (TotalHookDistance * TotalHookDistance)); 
	return SpeedCurve->GetFloatValue(CurveInput);
}

FVector UGrapplingHookComponent::GetDirection() const
{
	return (EndHookLocation - GetOwner()->GetActorLocation()).GetSafeNormal();
}

bool UGrapplingHookComponent::LookForGrabPoints(TSet<IGrabPoint*>& OutGrabPoints) const
{
	TArray<FHitResult> HitResults;
	if (PerformSphereTrace(HitResults))
	{
		for (auto HitResult : HitResults)
		{
			IGrabPoint* GrabPoint = Cast<IGrabPoint>(HitResult.GetActor());
			if (GrabPoint == nullptr)
			{
				FGvDebug::Error(HitResult.GetActor()->GetName() + " is not a grab point, FIX: set ignore collision response", true);
				continue;
			}
			// check if the grab point is in range and can be grabbed
			if (FVector::DistSquared(GetOwner()->GetActorLocation(), GrabPoint->GetLocation()) <= MaxDistance * MaxDistance
				&& GrabPoint->CanBeGrabbed(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
			{
				OutGrabPoints.Add(GrabPoint);
			}
			else
			{
				OutGrabPoints.Remove(GrabPoint);
			}
			
		}
		return true;
	}
	return false;
}

IGrabPoint* UGrapplingHookComponent::GetNearestGrabPoint(TSet<IGrabPoint*>& ValidGrabPoints) const
{
	IGrabPoint* ReturnValue = nullptr;
	float MinSquaredDistance = BIG_NUMBER;
	for (IGrabPoint* GrabPoint : ValidGrabPoints)
	{
		const float SquaredDistance = FVector::DistSquared(GrabPoint->GetLocation(), GetOwner()->GetActorLocation());
		if (MinSquaredDistance > SquaredDistance)
		{
			MinSquaredDistance = SquaredDistance;
			ReturnValue = GrabPoint;
		}
	}
	return ReturnValue;
}

float UGrapplingHookComponent::GetElapsedNormalizedDistance() const
{
	return FVector::DistSquared(GetOwner()->GetActorLocation(), StartHookLocation) / (TotalHookDistance * TotalHookDistance);
}


bool UGrapplingHookComponent::PerformSphereTrace(TArray<FHitResult>& HitResults) const
{
	const FCollisionShape CollisionShape = FCollisionShape::MakeSphere(MaxDistance);
	constexpr ECollisionChannel GrabPointsChannel = ECC_GameTraceChannel1;

	
	const bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults, 
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(),
		FQuat::Identity,
		GrabPointsChannel,
		CollisionShape
	);

#if WITH_EDITOR
	if (bShowDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			GetOwner()->GetActorLocation(),
			MaxDistance,
			15,
			bHit ? FColor::Green : FColor::Red,
			false,
			-1
		);
	}
#endif

	return bHit;
	
}
