// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Base/ResourceAttributeBase.h"
#include "HealthAttribute.generated.h"

/**
 * 
 */
UCLASS()
class GVUEDK_API UHealthAttribute : public UResourceAttributeBase
{
	GENERATED_BODY()

public:
	virtual void SetValue(const float NewValue) override;
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	UFUNCTION(BlueprintCallable)
	bool IsInvulnerable() const;
	UFUNCTION(BlueprintCallable)
	void SetInvulnerable(const bool Value);

private:
	bool bIsInvulnerable;
};
