// Copyright VUNDK, Inc. All Rights Reserved.

#include "UGameFramework/VUNDKControllerBase.h"

void AVUNDKControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindKey(EKeys::AnyKey, IE_Pressed, this, &AVUNDKControllerBase::OnAnyKeyPressed);
	InputComponent->BindKey(EKeys::AnyKey, IE_Released, this, &AVUNDKControllerBase::OnAnyKeyReleased);
}

void AVUNDKControllerBase::OnAnyKeyPressed(FKey Key)
{
	OnKeyPressed.Broadcast(Key);
}

void AVUNDKControllerBase::OnAnyKeyReleased(FKey Key)
{
	OnKeyReleased.Broadcast(Key);
}
