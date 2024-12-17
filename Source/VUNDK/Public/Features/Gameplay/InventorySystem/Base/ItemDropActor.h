// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "ItemDropActor.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VUNDK_API AItemDropActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	UItemBase* RelatedItem;
	
public:
	AItemDropActor();

	void Init(class UItemBase* Item);

private:
	bool Check() const;
};
