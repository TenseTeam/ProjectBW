// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/GrabPoint.h"
#include "Features/Gameplay/GrapplingHookSystem/GrapplingHookComponent.h"
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

	bool bInitialized;

public:
	ABWGrabPoint();
	virtual void BeginPlay() override;
	virtual FVector GetLocation() const override;
	virtual FVector GetLandingPoint() const override;
	virtual bool CanBeGrabbed(const ACharacter* Character) const override;

private:
	FVector GetPlayerCameraLocation() const;
	void Initialize();
};
