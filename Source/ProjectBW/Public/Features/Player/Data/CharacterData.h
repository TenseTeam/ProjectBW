// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Patterns/State/StateBase.h"
#include "CharacterData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBW_API UCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunAcceleration;
};
