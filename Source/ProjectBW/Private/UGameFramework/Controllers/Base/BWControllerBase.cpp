// Copyright Villains, Inc. All Rights Reserved.

#include "UGameFramework/Controllers/Base/BWControllerBase.h"

ABWControllerBase::ABWControllerBase()
{
	ContextsManager = CreateDefaultSubobject<UContextsManager>(TEXT("ContextsManager"));
	MenuManager = CreateDefaultSubobject<UMenuManager>(TEXT("MenuManager"));
}
