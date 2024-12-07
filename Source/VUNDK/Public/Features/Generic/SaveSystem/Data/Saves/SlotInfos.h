// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Features/Generic/SaveSystem/Data/Structs/SlotInfoData.h"
#include "SlotInfos.generated.h"

UCLASS()
class VUNDK_API USlotInfos : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FSlotInfoData> SlotInfos = TMap<FName, FSlotInfoData>();
};
