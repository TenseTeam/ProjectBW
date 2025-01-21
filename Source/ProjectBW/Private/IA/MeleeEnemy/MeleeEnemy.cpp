// Copyright Villains, Inc. All Rights Reserved.


#include "IA/MeleeEnemy/MeleeEnemy.h"

#include "Utility/LGDebug.h"


AMeleeEnemy::AMeleeEnemy()
{
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActor"));
	ChildActor->SetChildActorClass(AMeleeWeapon::StaticClass());
	
}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	MeleeWeapon = Cast<AMeleeWeapon>(ChildActor->GetChildActor());
	
	// if (bWieldSword)
	// {
	// 	PlayMontageWithNotify(EquipSwordMontage);
	// }
	
}

void AMeleeEnemy::PostInitProperties()
{
	Super::PostInitProperties();
	ChildActor->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,SwordSocketOnBack);
	
}

void AMeleeEnemy::EquipSword(FName SocketName)
{
	if (ChildActor && GetMesh())
	{
		ChildActor->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			SocketName 
		);
		
		LGDebug::Log("NavLocation: " + ChildActor->GetAttachSocketName().ToString(), true);
	}
}

void AMeleeEnemy::PlayMontageWithNotify(UAnimMontage* MontageToPlay)
{
	
	if (!MontageToPlay || !GetMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Montage or Mesh."));
		return;
	}

	LGDebug::Log(" INIZIO L'ANIMAZIONE : ",true);

	PlayAnimMontage(MontageToPlay);
	
	//AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMeleeEnemy::OnMontageNotifyReceived);	
		
	
}

void AMeleeEnemy::OnMontageNotifyReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	LGDebug::Log(" NOTIFY BEGIN : " + ChildActor->GetAttachSocketName().ToString(), true);
	
	if (NotifyName == "ChangeSocket" && ChildActor && GetMesh())
	{
		FName SocketName;
		
		if (bWieldSword)
		{
			SocketName = SwordSocketOnBack;
			bWieldSword = false;
		}
		else
		{
			SocketName = SwordSocketInHand;
			bWieldSword = true;
		}
		
		EquipSword(SocketName);
	}
}
