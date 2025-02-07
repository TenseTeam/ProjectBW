// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Features/Gameplay/GrapplingHookSystem/Interfaces/GrabPoint.h"
#include "GameFramework/Character.h"
#include "Dummy.generated.h"

UCLASS()
class PROJECTBW_API ADummy : public ACharacter, public IGrabPoint
{
	GENERATED_BODY()
};
