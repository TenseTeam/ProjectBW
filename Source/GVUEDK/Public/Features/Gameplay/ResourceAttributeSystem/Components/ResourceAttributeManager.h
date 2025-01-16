// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceAttributeManager.generated.h"


class UResourceAttributeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UResourceAttributeManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UResourceAttributeManager();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	bool TryGetAttribute(const FName AttributeName, UResourceAttributeBase*& OutAttribute) const;

private:
	void InitializeAttributes();

private:
	UPROPERTY(EditAnywhere, Instanced, meta = (AllowPrivateAccess = "true"))
	TMap<FName, UResourceAttributeBase*> AttributeMap;
};
