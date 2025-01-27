// Copyright Villains, Inc. All Rights Reserved.


#include "Features/Gameplay/GrapplingHookSystem/Behaviours/Base/GrapplingHookModeBase.h"

#include "Features/Gameplay/GrapplingHookSystem/Components/GrapplingHookComponent.h"

void UGrapplingHookModeBase::Initialize(UGrapplingHookComponent* InGrapplingHookComponent)
{
	GrapplingHookComponent = InGrapplingHookComponent;
	OwnerCharacter = InGrapplingHookComponent->GetOwnerCharacter();
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
