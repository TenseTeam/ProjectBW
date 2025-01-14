// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkCustomComponent.h" 
#include "CustomNavLinkProxy.generated.h"

UCLASS()
class LGUEDK_API ACustomNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	ACustomNavLinkProxy();
	
	virtual void BeginPlay() override;
	// // Check if the Nav Link is available (not occupied)
	// UFUNCTION(BlueprintCallable, Category = "NavLink")
	// bool IsAvailable() const;
	//
	// // Mark the Nav Link as occupied (used by an agent)
	// void MarkAsOccupied(float OccupyDuration);
	//
	// // Release the Nav Link (make it available again)
	// void Release();
	//
	// // Get the jump height required for this Nav Link (difference in Z coordinate)
	// float GetJumpHeightRequired() const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Custom Nav Link")
	TEnumAsByte<ENavLinkDirection::Type> AINavLinkDirection;

	// void OnMySmartLinkReached(AActor* Agent, const FVector& Destination);
	//
	// bool bIsOccupied;
	//
	// FTimerHandle OccupyTimerHandle;
	//
	// float JumpHeightRequired;

};
