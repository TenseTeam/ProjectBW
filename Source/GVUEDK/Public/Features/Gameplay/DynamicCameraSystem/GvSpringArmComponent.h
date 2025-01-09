// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "GvSpringArmComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GVUEDK_API UGvSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:

	UGvSpringArmComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Natural length of the spring arm when there are no collisions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float BaseTargetArmLength;

	/** Value to add to the target arm length, e.g. caused by camera modifiers. */
	float TargetArmLengthModifier;

protected:

	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;
	virtual FVector BlendLocations(const FVector& DesiredArmLocation, const FVector& TraceHitLocation, bool bHitSomething, float DeltaTime) override;

};
