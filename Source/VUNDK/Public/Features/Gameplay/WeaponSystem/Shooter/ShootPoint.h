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
	float Spread;
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UArrowComponent* ArrowComponent;
#endif

public:
	UShootPoint();

	UFUNCTION(BlueprintCallable)
	void SetSpread(const float NewSpread);
	
	UFUNCTION(BlueprintPure)
	FVector GetShootPointLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeLocation() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointDirection() const;

	UFUNCTION(BlueprintPure)
	FVector GetShootPointRelativeDirection() const;

protected:
	virtual void OnRegister() override;
};
