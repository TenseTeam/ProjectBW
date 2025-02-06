// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Interfaces/GrabPoint.h"
#include "GrapplingHookComponent.generated.h"

class UGHSearchModeBase;
class UGHMovementModeBase;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHookMotionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPerformHookMotion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopHooking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInterruptHooking);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UGrapplingHookComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FStartHooking OnStartHooking;
	UPROPERTY(BlueprintAssignable)
	FHookMotionStarted OnHookMotionStarted;
	UPROPERTY(BlueprintAssignable)
	FPerformHookMotion OnPerformHookMotion;
	UPROPERTY(BlueprintAssignable)
	FStopHooking OnStopHooking;
	UPROPERTY(BlueprintAssignable)
	FInterruptHooking OnInterruptHooking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UGHMovementModeBase*> MovementModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	TArray<UGHSearchModeBase*> SearchModes;

private:
	UPROPERTY()
	UGHMovementModeBase* CurrentMovementMode;
	UPROPERTY()
	UGHSearchModeBase* CurrentSearchMode;
	UPROPERTY()
	ACharacter* OwnerCharacter;
	
	IGrabPoint* TargetGrabPoint;
	bool bTargetAcquired;

	FVector HookLocation;
	bool bHookHitObstacle;
	
	bool bInitialized;

public:
	UGrapplingHookComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartHooking();
	UFUNCTION(BlueprintCallable)
	void StopHooking();
	
	UFUNCTION(BlueprintCallable)
	float GetMaxDistance() const;
	UFUNCTION(BlueprintCallable)
	float GetMinDistance() const;
	UFUNCTION(BlueprintCallable)
	float GetTotalHookDistance() const;
	UFUNCTION(BlueprintCallable)
	float GetElapsedTime() const;
	UFUNCTION(BlueprintCallable)
	float GetStartDelay() const;
	UFUNCTION(BlueprintCallable)
	FVector GetStartLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetLandingPointLocation() const;
	UFUNCTION(BlueprintCallable)
	FVector GetStartDirection() const;
	UFUNCTION(BlueprintCallable)
	FVector GetTargetGrabPointLocation() const { return bTargetAcquired ? TargetGrabPoint->Execute_GetLocation(TargetGrabPoint->_getUObject()) : FVector::ZeroVector; }
	UFUNCTION(BlueprintCallable)
	FVector GetHookLocation() const { return HookLocation; }
	
	
	UFUNCTION(BlueprintCallable)
	void ChangeMovementMode(int32 ModeIndex);
	UFUNCTION(BlueprintCallable)
	void ChangeSearchMode(int32 ModeIndex);

	ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }
	IGrabPoint* GetTargetGrabPoint() const { return TargetGrabPoint; }
	void SetTargetGrabPoint(IGrabPoint* GrabPoint) { TargetGrabPoint = GrabPoint; bTargetAcquired = TargetGrabPoint != nullptr; }
	void SetHookLocation(const FVector& Location) { HookLocation = Location; }
	void SetHookHitObstacle(const bool Value) { bHookHitObstacle = Value; }
	bool IsTargetAcquired() const { return bTargetAcquired; }
	bool HasHookHitObstacle() const { return bHookHitObstacle; }

private:
	// Returns true if the modes are initialized successfully
	bool InitializeModes();
	
	
};


