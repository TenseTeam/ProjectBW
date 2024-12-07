// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StatOperation.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API UStatOperation : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	UObject* Payload;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 ModifierValue;
	
public:
	void Init(UObject* InPayload, int32 InModifierValue);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	int32 GetResultOperation() const;
};
