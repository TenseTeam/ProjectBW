// Copyright Villains, Inc. All Rights Reserved.


#include "Components/TeamComponent/EnemyAttackRequestManager.h"

#include "Utility/LGDebug.h"


UEnemyAttackRequestManager::UEnemyAttackRequestManager()
{
	CurrentTokens = MaxTokens;
}


void UEnemyAttackRequestManager::BeginPlay()
{
	Super::BeginPlay();
	
}


bool UEnemyAttackRequestManager::TryGetToken(int TokenCost)
{
	if (CurrentTokens < TokenCost)
	{
		LGDebug::Log("Not enough tokens. Current tokens: %d" + CurrentTokens, true);
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






