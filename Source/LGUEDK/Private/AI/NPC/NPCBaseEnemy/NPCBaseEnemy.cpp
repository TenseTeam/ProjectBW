// Copyright Villains, Inc. All Rights Reserved.

#include "LGUEDK/Public/AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"



ANPCBaseEnemy::ANPCBaseEnemy()
{
	
}

void ANPCBaseEnemy::SetMovementSpeed(EMovementSpeed MovementSpeed) const
{

	UCharacterMovementComponent* MyCharacterMovement = GetCharacterMovement();
	if (!MyCharacterMovement)
	{
		LGDebug::Error("CharacterMovement component is null!",true);
		return;
	}
	
	switch (MovementSpeed)
	{
	case EMovementSpeed::Idle:
		MyCharacterMovement->MaxWalkSpeed = IdleSpeed; 
		break;

	case EMovementSpeed::Walk:
		MyCharacterMovement->MaxWalkSpeed = WalkSpeed; 
		break;

	case EMovementSpeed::Run:
		MyCharacterMovement->MaxWalkSpeed = RunSpeed; 
		break;

	case EMovementSpeed::Sprint:
		MyCharacterMovement->MaxWalkSpeed = SprintSpeed; 
		break;

	default:
		LGDebug::Error("EMovement speed invalid",true);
		break;
	}

	//LGDebug::Log(*StaticEnum<EMovementSpeed>()->GetNameByValue((int64)MovementSpeed).ToString(),true);
	
}

void ANPCBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}


