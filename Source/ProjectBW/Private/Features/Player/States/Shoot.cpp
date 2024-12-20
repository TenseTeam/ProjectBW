// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Shoot.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/FGvDebug.h"

void UShoot::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	Character->SetActorRotation(Controller->GetControlRotation());
	Character->ActionState = ECharacterState::Shooting;
	
	if (Character->WantAiming())
	{
		//bIsAiming = true;
		Character->SetIsAiming(true);
		Character->OnStartAiming.Broadcast();
	}
	if (Character->WantShooting())
	{
		//bIsShooting = true;
		Character->SetIsShooting(true);
		Character->OnStartShooting.Broadcast();
	}

	Character->OnStartRunning.AddDynamic(this, &UShoot::InterruptShoot);
	Character->OnStartDodging.AddDynamic(this, &UShoot::InterruptShoot);
	Character->OnStartHook.AddDynamic(this, &UShoot::InterruptShoot);
}

void UShoot::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);
	
	//FGvDebug::Warning(bPaused ? "Paused" : "Not Paused", true);
	
	//Temporary solution: need to refactor
	// if (!Character->WantAiming() && !Character->WantShooting())
	// {
	// 	Character->ChangeActionState(0);
	// 	return;
	// }
	//
	// if (!bPaused)
	// {
	// 	if (Character->WantAiming())
	// 	{
	// 		bIsAiming = true;
	// 		Character->OnAiming.Broadcast();
	// 	}
	// 	else if (bIsAiming)
	// 	{
	// 		bIsAiming = false;
	// 		Character->OnStopAiming.Broadcast();
	// 	}
	//
	// 	if (Character->WantShooting())
	// 	{
	// 		bIsShooting = true;
	// 		Character->OnShooting.Broadcast();
	// 	}
	// 	else if (bIsShooting)
	// 	{
	// 		bIsShooting = false;
	// 		Character->OnStopShooting.Broadcast();
	// 	}
	// }
	//
	// if (bPaused)
	// {
	// 	if (!Character->IsRunning() && !Character->IsDodging() && !Character->IsHooking())
	// 	{
	// 		UnpauseState();
	// 	}
	// }
	// else if (Character->IsRunning() || Character->IsDodging() || Character->IsHooking())
	// {
	// 	PauseState();
	// }

	if (Character->IsAiming())
	{
		Character->OnAiming.Broadcast();
	}
	if (Character->IsShooting())
	{
		Character->OnShooting.Broadcast();
	}
	
	// if (Character->IsDodging() || Character->IsHooking() || (!Character->WantAiming() && !Character->WantShooting()))
 //    {
 //        Character->ChangeActionState(0);
 //    }
}

void UShoot::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;

	if (Character->IsAiming())
	{
		//bIsAiming = false;
		Character->SetIsAiming(false);
		Character->OnStopAiming.Broadcast();
	}
	if (Character->IsShooting())
    {
		//bIsShooting = false;
		Character->SetIsShooting(false);
        Character->OnStopShooting.Broadcast();
    }

	Character->OnStartRunning.RemoveDynamic(this, &UShoot::InterruptShoot);
	Character->OnStartDodging.RemoveDynamic(this, &UShoot::InterruptShoot);
	Character->OnStartHook.RemoveDynamic(this, &UShoot::InterruptShoot);
}

void UShoot::HandleInput(AActor* Context, const EInputActionType InputAction, const FInputActionValue& Value)
{
	if (!bInitialized)
	{
		return;
	}
	Super::HandleInput(Context, InputAction, Value);
	
	// if (InputAction == EInputActionType::Shoot && !bIsShooting)
	// {
	// 	bIsShooting = true;
	// 	Character->OnStartShooting.Broadcast();
	// }
	// if (InputAction == EInputActionType::Aim && !bIsAiming)
	// {
	// 	bIsAiming = true;
	// 	Character->OnStartAiming.Broadcast();
	// }
	if (InputAction == EInputActionType::Shoot)
	{
		//bIsShooting = Value.Get<bool>();
		Character->SetIsShooting(Value.Get<bool>());
		if (Character->IsShooting())
		{
			Character->OnStartShooting.Broadcast();
		}
		else
		{
			Character->OnStopShooting.Broadcast();
			// if also not aiming, we want to go back to idle
			if (!Character->IsAiming())
			{
				Character->ChangeActionState(0);
				return;
			}
		}
	}
	if (InputAction == EInputActionType::Aim)
	{
		//bIsAiming = Value.Get<bool>();
		Character->SetIsAiming(Value.Get<bool>());
		if (Character->IsAiming())
		{
			Character->OnStartAiming.Broadcast();
		}
		else
		{
			Character->OnStopAiming.Broadcast();
			// if also not shooting, we want to go back to idle
			if (!Character->IsShooting())
			{
				Character->ChangeActionState(0);
				return;
			}
		}
	}
}

void UShoot::InterruptShoot()
{
	Character->ChangeActionState(0);
}

// void UShoot::PauseState()
// {
// 	bPaused = true;
// 	Character->GetCharacterMovement()->bOrientRotationToMovement = true;
// 	Character->GetCharacterMovement()->bUseControllerDesiredRotation = false;
// 	Character->ActionState = ECharacterState::None;
//
// 	if (Character->WantAiming())
// 	{
// 		bIsAiming = false;
// 		Character->OnStopAiming.Broadcast();
// 	}
// 	if (Character->WantShooting())
// 	{
// 		bIsShooting = false;
// 		Character->OnStopShooting.Broadcast();
// 	}
// }
//
// void UShoot::UnpauseState()
// {
// 	bPaused = false;
// 	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
// 	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
// 	Character->SetActorRotation(Controller->GetControlRotation());
// 	Character->ActionState = ECharacterState::Shooting;
//
// 	if (Character->WantAiming())
// 	{
// 		bIsAiming = true;
// 		Character->OnStartAiming.Broadcast();
// 	}
// 	if (Character->WantShooting())
// 	{
// 		bIsShooting = true;		
// 		Character->OnStartShooting.Broadcast();
// 	}
// }
