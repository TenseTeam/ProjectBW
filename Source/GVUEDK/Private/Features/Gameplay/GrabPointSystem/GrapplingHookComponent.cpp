// Copyright Villains, Inc. All Rights Reserved.


#include "GVUEDK/Public/Features/Gameplay/GrapplingHookSystem/GrapplingHookComponent.h"

#include <string>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"


UGrapplingHookComponent::UGrapplingHookComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTargetAcquired = false;
	bShowDebug = false;
	bIsHooking = false;
	bMotionDataCalculated = false;
	bOrientRotationToMovement = true;
	bApplyMomentumDuringHookThrow = false;
	bInitialized = false;
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
	OwnerCharacter = nullptr;
}

void UGrapplingHookComponent::BeginPlay()
{
	Super::BeginPlay();
	InRangeGrabPoints = TSet<IGrabPoint*>();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!IsValid(OwnerCharacter))
	{
		FGvDebug::Error(GetOwner()->GetName() + " is not a character, " + GetName() + " need to be on a character to work properly", true);
		bInitialized = false;
		return;
	}
	bInitialized = true;
}

void UGrapplingHookComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	if (!bInitialized)
	{
		return;
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsHooking)
	{
		if (bOrientRotationToMovement)
		{
			OrientRotationToMovement(DeltaTime);
		}
		
		if (ElapsedTime < StartDelay)
		{
			ElapsedTime += DeltaTime;
			return;
		}
		
		if (!bMotionDataCalculated)
		{
			if (CalculateMotionData())
			{
				bMotionDataCalculated = true;
				OnHookMotionStarted.Broadcast();
			}
			else return;
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
	bIsHooking = true;
	if (!bApplyMomentumDuringHookThrow)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = 0.f;
		OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
	OnStartHooking.Broadcast();
}

void UGrapplingHookComponent::StopHooking()
{
	TargetGrabPoint = nullptr;
	bTargetAcquired = false;
	bMotionDataCalculated = false;
	bIsHooking = false;
	OwnerCharacter->GetCharacterMovement()->GravityScale = 1.f;
	OnStopHooking.Broadcast();
}

void UGrapplingHookComponent::PerformHooking(float DeltaTime)
{
	OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + StartHookDirection * GetSpeed() * DeltaTime);
	OnHooking.Broadcast();
	if (GetElapsedNormalizedDistance() >= 1)
	{
		OwnerCharacter->SetActorLocation(EndHookLocation);
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
			// check if the grab point is in range and can be grabbed (TO DO: Check if DistSquared can be moved in CanBeGrabbed)
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

void UGrapplingHookComponent::OrientRotationToMovement(float DeltaTime) const
{
	FRotator TargetRotation = (TargetGrabPoint->GetLandingPoint() - GetOwner()->GetActorLocation()).Rotation();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;
	const FRotator NewRotation = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), TargetRotation, DeltaTime, 1000.f);
	GetOwner()->SetActorRotation(NewRotation);
}

bool UGrapplingHookComponent::CalculateMotionData()
{
	// check if there are any obstacles between the player and the target grab point
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(Cast<AActor>(TargetGrabPoint));
	GetWorld()->LineTraceMultiByChannel(HitResults, GetOwner()->GetActorLocation(), TargetGrabPoint->GetLocation(), ECC_Visibility, CollisionParams);
	if (HitResults.Num() > 0)
	{
		OnInterruptHooking.Broadcast();
		StopHooking();
		return false;
	}
	
	StartHookLocation = GetOwner()->GetActorLocation();
	EndHookLocation = TargetGrabPoint->GetLandingPoint();
	TotalHookDistance = FVector::Dist(StartHookLocation, EndHookLocation);
	StartHookDirection = (EndHookLocation - StartHookLocation).GetSafeNormal();
	return true;
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
