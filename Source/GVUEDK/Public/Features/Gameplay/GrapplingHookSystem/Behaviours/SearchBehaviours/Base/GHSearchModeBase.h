// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Behaviours/Base/GrapplingHookModeBase.h"
#include "GHSearchModeBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class GVUEDK_API UGHSearchModeBase : public UGrapplingHookModeBase
{
	GENERATED_BODY()

public:
	virtual float GetMaxDistance() const { return 0.f; }
	virtual float GetMinDistance() const { return 0.f; }
};
