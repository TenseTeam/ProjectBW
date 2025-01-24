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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	
public:
	AItemDropActor();

	void Init(UItemBase* Item);

private:
	bool Check() const;
};
