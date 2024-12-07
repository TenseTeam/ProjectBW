// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/RPGSystem/CharacterStats/CharacterStatsBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CharacterStatsUtility.generated.h"

UCLASS()
class VUNDK_API UCharacterStatsUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UCharacterStatsBase* CharacterStatsBase;

	void static Init(UCharacterStatsBase* InCharacterStatsBase);
	
	UFUNCTION(BlueprintPure, Category = "Character Stats")
	static UCharacterStatsBase* GetCharacterStats();
	
private:
	static bool Check();
};
