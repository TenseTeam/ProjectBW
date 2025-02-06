// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/GrapplingHookSystem/Behaviours/MovementBehaviours/GHTargetTowardSelf.h"

#include <string>

#include "Curves/CurveVector.h"
#include "Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"

UGHTargetTowardSelf::UGHTargetTowardSelf()
{
	bIsHooking = false;
	bMotionDataCalculated = false;
	bApplyMomentumDuringHookThrow = false;
	bMaxExtensionReached = false;
	bShowDebug = false;
	MaxHookExtension = 2000.f;
	HookLinearSpeed = 2000.f;
	StartDelay = 0.f;
	ElapsedTime = 0.f;
	HookSpeedCurve = nullptr;
	ExtensionDirection = FVector::ZeroVector;
	AimTraceChannel = ECC_Visibility;
}

void UGHTargetTowardSelf::StartHooking()
{
	Super::StartHooking();
	if (bIsHooking)
	{
		return;
	}
	
	bIsHooking = true;
	ElapsedTime = 0.f;
	bMaxExtensionReached = false;
	bMotionDataCalculated = false;
	ExtensionDirection = GetAimDirection();
	GrapplingHookComponent->SetHookLocation(GetHookStartLocationSocket());
	
	if (!bApplyMomentumDuringHookThrow)
	{
		CachedGravityScale = OwnerCharacter->GetCharacterMovement()->GravityScale;
		OwnerCharacter->GetCharacterMovement()->GravityScale = 0.f;
		OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

	//orient character rotation to the direction of the hook motion
	FRotator NewRotation = ExtensionDirection.Rotation();
	NewRotation.Pitch = 0.f;
	OwnerCharacter->SetActorRotation(NewRotation);
	
	GrapplingHookComponent->OnStartHooking.Broadcast();
}

void UGHTargetTowardSelf::StopHooking()
{
	Super::StopHooking();
	GrapplingHookComponent->SetHookHitObstacle(false);
	GrapplingHookComponent->SetTargetGrabPoint(nullptr);
	bIsHooking = false;
	TargetGrabbedLocation = FVector::ZeroVector;
	
	if (!bApplyMomentumDuringHookThrow)
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = CachedGravityScale;
	}
	
	GrapplingHookComponent->OnStopHooking.Broadcast();
}

bool UGHTargetTowardSelf::TickMode(float DeltaTime)
{
	Super::TickMode(DeltaTime);
	if (bIsHooking)
	{
		//OwnerCharacter->GetCharacterMovement()->GravityScale = 0.f; //debug
		//OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector; //debug
		
		if (ElapsedTime < StartDelay)
		{
			ElapsedTime += DeltaTime;
			return true; // do not want serch mode to tick before the start of hook motion
		}

		if (!bMotionDataCalculated) // calculate motion data only once
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
				return true;
			}
		}
		
		if (IsTargetAcquired())
		{
			// snap the hook to the target during the first frame after the target is acquired
			if (!bMaxExtensionReached) 
			{
				GrapplingHookComponent->SetHookLocation(GetTargetGrabPoint()->Execute_GetLocation(GetTargetGrabPoint()->_getUObject()));
				TargetGrabbedLocation = GrapplingHookComponent->GetHookLocation();
			}
			
			bMaxExtensionReached = true;
			GetTargetGrabPoint()->Execute_SetLocation(GetTargetGrabPoint()->_getUObject(), GrapplingHookComponent->GetHookLocation());
			if (bOrientGrabbedActorRotationToMovement)
				OrientGrabbedActorRotationToMovement();
		}
		else if (GrapplingHookComponent->HasHookHitObstacle())
        {
			bMaxExtensionReached = true;
        }
		
#if !UE_BUILD_SHIPPING
		if (bShowDebug)
		{
			DrawDebugBox(OwnerCharacter->GetWorld(), GrapplingHookComponent->GetHookLocation(), FVector(30.f), FColor::Blue, false, 0.01f);
			DrawDebugLine(OwnerCharacter->GetWorld(), GetHookStartLocationSocket(), GrapplingHookComponent->GetHookLocation(), FColor::Purple, false, 0.01f);
		}
#endif
		
		return PerformMotion(DeltaTime);
	}
	return true; 
}

bool UGHTargetTowardSelf::TryCalculateMotionData()
{
	return true;
}

float UGHTargetTowardSelf::GetHookSpeed() const
{
	if (!IsValid(HookSpeedCurve))
	{
		return HookLinearSpeed;
	}

	if (bMaxExtensionReached)
	{
		if (IsTargetAcquired())
			// speed for the way back motion with target
			return HookSpeedCurve->GetVectorValue(GetCurrentHookExtensionNormalized()).Y * 100.f; // convert from m/s to cm/s

		// speed for the way back motion without target
		return HookSpeedCurve->GetVectorValue(GetCurrentHookExtensionNormalized()).Z * 100.f; // convert from m/s to cm/s
	}

	// speed for the way out motion
	return HookSpeedCurve->GetVectorValue(GetCurrentHookExtensionNormalized()).X * 100.f; // convert from m/s to cm/s
}

float UGHTargetTowardSelf::GetCurrentHookExtensionNormalized() const
{
	float MaxExtensionSquared = MaxHookExtension * MaxHookExtension;
	if (TargetGrabbedLocation != FVector::ZeroVector)
	{
		MaxExtensionSquared = FVector::DistSquared(GetHookEndLocationSocket(), TargetGrabbedLocation);
	}
	return FVector::DistSquared(GrapplingHookComponent->GetHookLocation(), bMaxExtensionReached ? GetHookEndLocationSocket() : GetHookStartLocationSocket()) / MaxExtensionSquared;
}

FTransform UGHTargetTowardSelf::GetOwnerTransform() const
{
	FTransform Transform = OwnerCharacter->GetTransform();
	Transform.SetRotation(FQuat::Identity);
	return Transform;
}

FVector UGHTargetTowardSelf::GetHookStartLocationSocket() const
{
	return OwnerCharacter->GetMesh()->GetSocketLocation(HookStartLocationSocket);
}

FVector UGHTargetTowardSelf::GetHookEndLocationSocket() const
{
    return OwnerCharacter->GetMesh()->GetSocketLocation(HookEndLocationSocket);
}

FVector UGHTargetTowardSelf::GetAimDirection() const
{
	FVector AimDirection;
	const FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(OwnerCharacter ,0)->GetCameraCacheView().Location;
	const FVector StartTraceLocationWithOffset = CameraLocation + OwnerCharacter->GetControlRotation().Vector() * 200.f;
	const FVector PointedLocation = CameraLocation + OwnerCharacter->GetControlRotation().Vector() * MaxHookExtension;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerCharacter);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTraceLocationWithOffset, PointedLocation, AimTraceChannel, CollisionParams))
	{
		AimDirection = (HitResult.ImpactPoint - GetHookStartLocationSocket()).GetSafeNormal();
	}
	else AimDirection = (PointedLocation - GetHookStartLocationSocket()).GetSafeNormal();

#if !UE_BUILD_SHIPPING
	if (bShowDebug)
	{
		if (HitResult.bBlockingHit)
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20.f, FColor::Red, false, 5.f);
		DrawDebugLine(GetWorld(), StartTraceLocationWithOffset, PointedLocation, FColor::Green, false, 5.f);
	}
#endif

	return AimDirection;
}

bool UGHTargetTowardSelf::IsMaxExtensionReached() const
{
	return bMaxExtensionReached || GetCurrentHookExtensionNormalized() >= 1.f;
}

bool UGHTargetTowardSelf::IsMinExtensionReached(const float DeltaTime) const
{
	const float SquaredTolerance = (GetHookSpeed() * DeltaTime) * (GetHookSpeed() * DeltaTime);
	return FMath::IsNearlyZero(FVector::DistSquared(GrapplingHookComponent->GetHookLocation(), GetHookEndLocationSocket()), SquaredTolerance);
}

bool UGHTargetTowardSelf::PerformMotion(float DeltaTime)
{
	GrapplingHookComponent->OnPerformHookMotion.Broadcast();
	
	FVector NewHookLocation;
	if (IsMaxExtensionReached()) 
	{
		bMaxExtensionReached = true;
			
		const FVector ReductionDirection = (GetHookEndLocationSocket() - GrapplingHookComponent->GetHookLocation()).GetSafeNormal();
		NewHookLocation = GrapplingHookComponent->GetHookLocation() + ReductionDirection * GetHookSpeed() * DeltaTime;
		GrapplingHookComponent->SetHookLocation(NewHookLocation);
			
		if (IsMinExtensionReached(DeltaTime))
		{
			StopHooking();
			return true; // do not want search mode to tick after the end of hook motion
		}

		if (bCanGrabTargetOnHookReduction)
		{
			return IsTargetAcquired();
		}
		return true;
	}
	
	NewHookLocation = GrapplingHookComponent->GetHookLocation() + ExtensionDirection * GetHookSpeed() * DeltaTime;
	GrapplingHookComponent->SetHookLocation(NewHookLocation);
	return false; // continue search mode tick
}

void UGHTargetTowardSelf::OrientGrabbedActorRotationToMovement() const 
{
	const FVector MoveDirection = (OwnerCharacter->GetActorLocation() - GetTargetGrabPoint()->Execute_GetLocation(GetTargetGrabPoint()->_getUObject())).GetSafeNormal();
	FRotator MovementRotation = MoveDirection.Rotation();
	MovementRotation.Pitch = 0.f;
	GetTargetGrabPoint()->Execute_GetActor(GetTargetGrabPoint()->_getUObject())->SetActorRotation(MovementRotation);
}
