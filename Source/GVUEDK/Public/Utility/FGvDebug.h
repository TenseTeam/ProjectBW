// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class GVUEDK_API FGvDebug
{
public:
	FGvDebug();
	~FGvDebug();

	static void Log(const FString& LogMsg, const bool bPrintOnScreen = false/*, const FLogCategoryBase& LogCategory = LogTemp*/);
	static void Warning(const FString& WarningMsg, const bool bPrintOnScreen = false/*, const FLogCategoryBase& LogCategory = LogTemp*/);
	static void Error(const FString& ErrorMsg, const bool bPrintOnScreen = false/*, const FLogCategoryBase& LogCategory = LogTemp*/);
	
};
