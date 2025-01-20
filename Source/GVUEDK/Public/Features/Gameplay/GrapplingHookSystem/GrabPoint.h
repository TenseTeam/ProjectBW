// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrabPoint.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGrabPoint : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GVUEDK_API IGrabPoint
{
	GENERATED_BODY()

public:
	
	virtual FVector GetLandingPoint() const = 0;
	virtual FVector GetLocation() const = 0;
	virtual bool CanBeGrabbed(const ACharacter* Character) const = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Highlight();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Unhighlight();
};
