// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatDataBase.generated.h"

UCLASS()
class VUNDK_API UStatDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FName StatID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MaxLength = 3))
	FName StatCodeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatFullName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = true))
	FText StatDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 StatMinValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "!bIsUncapped", EditConditionHides))
	int32 StatMaxValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsUncapped;

public:
	UStatDataBase(): StatID("NotGeneratedID"),
	                 StatCodeName("AAA"),
	                 StatFullName(),
	                 StatDescription(),
	                 StatMinValue(0),
	                 StatMaxValue(1),
	                 bIsUncapped(false)
	{
	}

	FORCEINLINE virtual void PostInitProperties() override
	{
		Super::PostInitProperties();
		StatID = FName(FGuid::NewGuid().ToString());
	}
};
