// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/GrapplingHookSystem/Behaviours/MovementBehaviours/GHSelfTowardTarget.h"

#include "Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGHSelfTowardTarget::UGHSelfTowardTarget()
{
	bIsHooking = false;
	bMotionDataCalculated = false;
	bOrientRotationToMovement = true;
	bApplyMomentumDuringHookThrow = false;
	LinearSpeed = 2500.f;
	TotalHookDistance = 0.f;
	StartDelay = 0.f;
	ElapsedTime = 0.f;
	StartHookLocation = FVector::ZeroVector;
	EndHookLocation = FVector::ZeroVector;
	StartHookDirection = FVector::ZeroVector;
	SpeedCurve = nullptr;
}

void UGHSelfTowardTarget::StartHooking()
{
	Super::StartHooking();
	if (!IsTargetAcquired() || bIsHooking)
	{
		return;
	}
	ElapsedTime = 0.f;
	bIsHooking = true;
	if (!bApplyMomentumDuringHookThrow)
	{
		CachedGravityScale = OwnerCharacter->GetCharacterMovement()->GravityScale;
		OwnerCharacter->GetCharacterMovement()->GravityScale = 0.f;
		OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
	GetTargetGrabPoint()->Execute_Unhighlight(GetTargetGrabPoint()->_getUObject());
	GrapplingHookComponent->OnStartHooking.Broadcast();
}

void UGHSelfTowardTarget::StopHooking()
{
	Super::StopHooking();
	GrapplingHookComponent->SetTargetGrabPoint(nullptr);
	bMotionDataCalculated = false;
	bIsHooking = false;
	if (!bApplyMomentumDuringHookThrow)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = CachedGravityScale;
	}
	GrapplingHookComponent->OnStopHooking.Broadcast();
}

bool UGHSelfTowardTarget::TickMode(float DeltaTime)
{
	Super::TickMode(DeltaTime);
	if (bIsHooking)
	{
		if (bOrientRotationToMovement)
		{
			OrientRotationToMovement(DeltaTime);
		}
		
		if (ElapsedTime < StartDelay)
		{
			ElapsedTime += DeltaTime;
			return true;
		}
		
		if (!bMotionDataCalculated)
		{
			if (TryCalculateMotionData())
			{
				bMotionDataCalculated = true;
				GrapplingHookComponent->OnHookMotionStarted.Broadcast();
			}
			else
			{
				GrapplingHookComponent->OnInterruptHooking.Broadcast();
				StopHooking();
				return false;
			}
		}
		
		PerformMotion(DeltaTime);
		return true;
	}
	return false;
}

void UGHSelfTowardTarget::PerformMotion(float DeltaTime)
{
	OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	OwnerCharacter->SetActorLocation(OwnerCharacter->GetActorLocation() + StartHookDirection * GetSpeed() * DeltaTime);
	GrapplingHookComponent->OnPerformHookMotion.Broadcast();
	if (GetElapsedNormalizedDistance() >= 1)
	{
		OwnerCharacter->SetActorLocation(EndHookLocation);
		StopHooking();
	}
}

void UGHSelfTowardTarget::OrientRotationToMovement(float DeltaTime)
{
	const FVector LandingPoint = GetTargetGrabPoint()->Execute_GetLandingPoint(GetTargetGrabPoint()->_getUObject());
	FRotator TargetRotation = (LandingPoint - OwnerCharacter->GetActorLocation()).Rotation();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;
	const FRotator NewRotation = FMath::RInterpConstantTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 1000.f);
	OwnerCharacter->SetActorRotation(NewRotation);
}

bool UGHSelfTowardTarget::TryCalculateMotionData()
{
	// check if there are any obstacles between the player and the target grab point
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	AActor* TargetGrabPointActor = Cast<AActor>(GetTargetGrabPoint());
	CollisionParams.AddIgnoredActor(TargetGrabPointActor);
	CollisionParams.AddIgnoredActor(OwnerCharacter);
	GetWorld()->LineTraceMultiByChannel(HitResults, OwnerCharacter->GetActorLocation(), GetTargetGrabPoint()->Execute_GetLocation(TargetGrabPointActor), ECC_Visibility, CollisionParams);
	if (HitResults.Num() > 0)
	{
		return false;
	}
	
	StartHookLocation = OwnerCharacter->GetActorLocation();
	EndHookLocation = GetTargetGrabPoint()->Execute_GetLandingPoint(GetTargetGrabPoint()->_getUObject());
	TotalHookDistance = FVector::Dist(StartHookLocation, EndHookLocation);
	StartHookDirection = (EndHookLocation - StartHookLocation).GetSafeNormal();
	return true;
}

float UGHSelfTowardTarget::GetElapsedNormalizedDistance()
{
	return FVector::DistSquared(OwnerCharacter->GetActorLocation(), StartHookLocation) / (TotalHookDistance * TotalHookDistance);
}

float UGHSelfTowardTarget::GetSpeed()
{
	if (!IsValid(SpeedCurve))
	{
		return LinearSpeed;
	}
	const float CurveInput = SpeedCurve->GetFloatValue(FVector::DistSquared(OwnerCharacter->GetActorLocation(), StartHookLocation) / (TotalHookDistance * TotalHookDistance)); 
	return SpeedCurve->GetFloatValue(CurveInput);
}
