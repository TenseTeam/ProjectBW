// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Saver.h"
#include "TransformSaver.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VUNDK_API UTransformSaver : public USaver
{
	GENERATED_BODY()

public:
	UTransformSaver();
	
	virtual void OnPrepareSave_Implementation(UDefaultSaveGame* SaveGameData, USlotInfoItem* SlotInfoItem, UObject* Instigator) override;

protected:
	virtual void OnLoadCompletedEvent_Implementation(const FString& SlotName, const int32 UserIndex, UDefaultSaveGame* LoadedData, UObject* Instigator) override;

	UFUNCTION(BlueprintCallable)
	FTransform GetOwnerTransform() const;

	virtual void BeginPlay() override;
	
private:
	void SetActorLoadedTransform(UDefaultSaveGame* LoadedData) const;
};
