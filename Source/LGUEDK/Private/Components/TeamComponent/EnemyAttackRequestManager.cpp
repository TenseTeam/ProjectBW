// Copyright Villains, Inc. All Rights Reserved.


#include "Components/TeamComponent/EnemyAttackRequestManager.h"

#include "Utility/LGDebug.h"


UEnemyAttackRequestManager::UEnemyAttackRequestManager()
{
	
}


void UEnemyAttackRequestManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentTokens = MaxTokens;
}


bool UEnemyAttackRequestManager::TryGetToken(int TokenCost)
{
	if (CurrentTokens < TokenCost)
	{
		LGDebug::Log("Not enough tokens. Current tokens:" + CurrentTokens, true);
		return false;
	}
	
	CurrentTokens -= TokenCost;
	LGDebug::Log(" CurrentTokens: " + CurrentTokens, true);
	LGDebug::Log(" TokenCost: " + TokenCost, true);
	return true;
}

void UEnemyAttackRequestManager::ReleaseToken(int ReturnToken)
{
	if (CurrentTokens >= MaxTokens)
		return;

	CurrentTokens += ReturnToken;
		
	LGDebug::Log("TOKEN TORNATO " + CurrentTokens, true);
}






