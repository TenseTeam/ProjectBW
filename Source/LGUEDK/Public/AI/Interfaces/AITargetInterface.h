// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AITargetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAITargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LGUEDK_API IAITargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// virtual void ReleaseToken() = 0; ASTRATTO = 0;
	virtual void ReleaseToken();
	
};
