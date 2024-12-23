// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EnemyBase/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"


// Sets default values
AEnemyBase::AEnemyBase()
{
	AIControllerClass = AEnemyControllerBase::StaticClass();
}

UBehaviorTree* AEnemyBase::GetBehaviorTree() const
{
	return BehaviorTree;
}

void AEnemyBase::SetMovementSpeed(EMovementSpeed MovementSpeed)
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

	LGDebug::Log(*StaticEnum<EMovementSpeed>()->GetNameByValue((int64)MovementSpeed).ToString(),true);
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}



