// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Generic/SaveSystem/Data/SaveData.h"
#include "UObject/Object.h"
#include "ISaveable.generated.h"

UINTERFACE(BlueprintType)
class VUNDK_API USaveable : public UInterface
{
	GENERATED_BODY()
};

class ISaveable
{
	GENERATED_BODY()

public:
	virtual USaveData* CreateSaveData() = 0;
    
	virtual bool LoadSaveData(USaveData* SavedData) = 0;
};
