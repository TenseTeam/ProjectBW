// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Player/States/ActionStates/Reload.h"

#include "Utility/FGvDebug.h"

void UReload::Initialize(AActor* Context)
{
	Super::Initialize(Context);
	if (!IsValid(Character->Data))
	{
		bInitialized = false;
	}
}

void UReload::Enter(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Enter(Context);

	ReloadDuration = Character->Data->ReloadDuratioin;
	
	if (ReloadDuration > 0)
	{
		Character->GetWorldTimerManager().ClearTimer(ReloadTimer);
		Character->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UReload::ReloadCompleted, ReloadDuration, false);
		Character->OnStartHook.AddDynamic(this, &UReload::InterruptReload);
		Character->OnStartDodging.AddDynamic(this, &UReload::InterruptReload);
		Character->SetIsReloading(true);
		Character->OnStartReloading.Broadcast();
	}
	else
	{
		ReloadCompleted();
	}
	
}

void UReload::Update(AActor* Context, float DeltaTime)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Update(Context, DeltaTime);
	
	if (ReloadDuration > 0)
	{
		Character->OnReloading.Broadcast();
	}
}

void UReload::Exit(AActor* Context)
{
	if (!bInitialized)
	{
		return;
	}
	Super::Exit(Context);
	Character->SetIsReloading(false);
	if (ReloadDuration > 0)
	{
		Character->OnStopReloading.Broadcast();
		Character->OnStartHook.RemoveDynamic(this, &UReload::InterruptReload);
		Character->OnStartDodging.RemoveDynamic(this, &UReload::InterruptReload);
	}
}

void UReload::InterruptReload()
{
	Character->GetWorldTimerManager().ClearTimer(ReloadTimer);
	Character->SetIsReloading(false);
	Character->OnReloadInterrupted.Broadcast();
	ReloadCompleted();
}

void UReload::ReloadCompleted()
{
	if ((Character->WantAiming() || Character->WantShooting()) && Character->CanShoot())
	{
		Character->ChangeActionState(1);
	}
	else
	{
		Character->ChangeActionState(0);
	}
}
