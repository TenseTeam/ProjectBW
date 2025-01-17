// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "ShootPoint.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), NotBlueprintable)
class VUNDK_API UShootPoint : public USceneComponent
{
	GENERATED_BODY()

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

public:
	UShootPoint();

	UFUNCTION(BlueprintPure)
	FVector GetShootPointLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointDirection() const;
};
