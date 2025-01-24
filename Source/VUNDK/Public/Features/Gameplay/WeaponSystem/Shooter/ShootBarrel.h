// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShootPoint.h"
#include "Components/SceneComponent.h"
#include "ShootBarrel.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UShootBarrel : public USceneComponent
{
	GENERATED_BODY()

public:
	UShootBarrel();

	UFUNCTION(BlueprintPure)
	TArray<UShootPoint*> GetShootPointsChildren() const;
	
protected:
	virtual void BeginPlay() override;
};
