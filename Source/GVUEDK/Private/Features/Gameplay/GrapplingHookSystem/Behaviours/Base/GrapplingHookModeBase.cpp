// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/GrapplingHookSystem/Behaviours/Base/GrapplingHookModeBase.h"

#include "Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"

void UGrapplingHookModeBase::Initialize(UGrapplingHookComponent* InGrapplingHookComponent)
{
	GrapplingHookComponent = InGrapplingHookComponent;
	OwnerCharacter = InGrapplingHookComponent->GetOwnerCharacter();
}

void UGrapplingHookModeBase::EnterMode()
{
	GrapplingHookComponent->SetHookHitObstacle(false);
	GrapplingHookComponent->SetTargetGrabPoint(nullptr);
}

void UGrapplingHookModeBase::ExitMode()
{
	GrapplingHookComponent->SetHookHitObstacle(false);
	GrapplingHookComponent->SetTargetGrabPoint(nullptr);
}

IGrabPoint* UGrapplingHookModeBase::GetTargetGrabPoint() const
{
	return GrapplingHookComponent->GetTargetGrabPoint();
}

void UGrapplingHookModeBase::SetTargetGrabPoint(IGrabPoint* GrabPoint)
{
	GrapplingHookComponent->SetTargetGrabPoint(GrabPoint);
}

bool UGrapplingHookModeBase::IsTargetAcquired() const
{
	return GrapplingHookComponent->IsTargetAcquired();
}
