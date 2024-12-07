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
	UPROPERTY(BlueprintReadOnly, Category = "VUNDK|Input")
	FOnPlayerControllerTick OnTick;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Input")
	FOnAnyKeyPressed OnKeyPressed;
	UPROPERTY(BlueprintAssignable, Category = "VUNDK|Input")
	FOnAnyKeyReleased OnKeyReleased;

public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void SetupInputComponent() override;

private:
	void OnAnyKeyPressed(FKey Key);

	void OnAnyKeyReleased(FKey Key);
};
