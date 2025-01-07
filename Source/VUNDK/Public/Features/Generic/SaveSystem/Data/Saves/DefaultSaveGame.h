// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Features/Generic/SaveSystem/Data/Structs/SlotInfoData.h"
#include "DefaultSaveGame.generated.h"

UCLASS(BlueprintType, Blueprintable)
class VUNDK_API UDefaultSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName SlotNameKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FTransform> ActorTransforms;

	UDefaultSaveGame(): SlotNameKey(""),
	                    ActorTransforms(TMap<FName, FTransform>())
	{
	}

	void SetSlotNameKey(const FName& Key)
	{
		SlotNameKey = Key;
	}
};
