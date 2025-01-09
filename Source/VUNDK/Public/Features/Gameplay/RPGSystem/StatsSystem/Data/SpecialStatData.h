// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Features/Gameplay/RPGSystem/StatsSystem/StatOperation.h"
#include "SpecialStatData.generated.h"

UCLASS(BlueprintType)
class VUNDK_API USpecialStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName StatID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor StatColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* StatIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StatDefaultValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStatOperation> StatBaseValueOperationClass;

public:
	USpecialStatData(): StatID("NotGeneratedID"),
	                    StatCodeName("AAA"),
	                    StatColor(),
	                    StatIcon(nullptr),
	                    StatDefaultValue(10)
	{
	}

	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		StatID = FName(FGuid::NewGuid().ToString());
	}
};
