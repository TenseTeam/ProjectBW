// Copyright VUNDK, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Features/Generic/SaveSystem/SaveManager.h"
#include "VUNDKGameInstance.generated.h"

UCLASS()
class VUNDK_API UVUNDKGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
	FSaveManagerData SaveManagerData;
	UPROPERTY(BlueprintReadOnly, Category = "Save System")
	USaveManager* SaveManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation")
	bool bTabNavigation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation")
	bool bAnalogNavigation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Navigation")
	bool bKeyNavigation = true;
	
public:
	virtual void Init() override;

private:
	void SetSlateNavigationConfigs() const;
};
