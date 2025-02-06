// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Interfaces/GrabPoint.h"
#include "Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"
#include "GameFramework/Actor.h"
#include "BWGrabPoint.generated.h"

UCLASS()
class PROJECTBW_API ABWGrabPoint : public AActor, public IGrabPoint
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = true, AllowPrivateAccess = "true"))
	FVector LandingPoint;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;
	UPROPERTY()
	UGrapplingHookComponent* PlayerGrapplingHook;

	FCollisionQueryParams TraceParams;
	bool bInitialized;

public:
	ABWGrabPoint();
	virtual void BeginPlay() override;
	virtual FVector GetLocation_Implementation() override;
	virtual FVector GetLandingPoint_Implementation() override;
	virtual bool CanBeGrabbed_Implementation(const ACharacter* Character) override;

private:
	FVector GetPlayerCameraLocation() const;
	bool ObstacleInBetween(const ACharacter* Character, FHitResult& HitResult);
	void Initialize();
};
