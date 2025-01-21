// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/Shoot.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/FGvDebug.h"

void UShoot::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	if (!IsValid(Character->Data))
	{
		FGvDebug::Warning("Shoot state has missing required data", true);
		bInitialized = false;
		return;
	}
	bInitialized = true;
}

void UShoot::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	RotationSpeed = Character->Data->AimingRotationSpeed;
	TargetRotation = FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f);
	CurrentRotation = Character->GetActorRotation();
	
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

	if (Character->IsAiming())
	{
		Character->OnAiming.Broadcast();
	}
	
	
	if (!CurrentRotation.EqualsOrientation(TargetRotation, 0.1f))
	{
		CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		Character->SetActorRotation(CurrentRotation);
		return;
	}
	Character->SetActorRotation(FRotator(0.f, Controller->GetControlRotation().Yaw, 0.f));

	
	if (Character->IsShooting())
	{
		Character->OnShooting.Broadcast();
	}
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

	//Handle shoot input
	if (InputAction == EInputActionType::Shoot)
	{
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

	//Handle aim input
	if (InputAction == EInputActionType::Aim)
	{
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

	if (InputAction == EInputActionType::Interact)
	{
		if (Character->IsShooting()) return;
		Character->ChangeActionState(2);
		return;
	}
}

void UShoot::InterruptShoot()
{
	Character->ChangeActionState(0);
}
