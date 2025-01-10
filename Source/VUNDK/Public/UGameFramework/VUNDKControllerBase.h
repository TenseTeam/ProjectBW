// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VUNDKControllerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyKeyPressed,
	FKey, Key
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAnyKeyReleased,
	FKey, Key
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnPlayerControllerTick,
	float, DeltaTime
);

UCLASS(Abstract, Blueprintable, BlueprintType)
class VUNDK_API AVUNDKControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAnyKeyPressed OnKeyPressed;
	UPROPERTY(BlueprintAssignable)
	FOnAnyKeyReleased OnKeyReleased;

protected:
	virtual void SetupInputComponent() override;

private:
	void OnAnyKeyPressed(FKey Key);

	void OnAnyKeyReleased(FKey Key);
};
