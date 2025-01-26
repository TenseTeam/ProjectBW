// Copyright Villains, Inc. All Rights Reserved.

#include "UGameFramework/Controllers/Base/BWControllerBase.h"

#include "Features/Generic/SaveSystem/Components/Savers/Saver.h"

ABWControllerBase::ABWControllerBase()
{
	ContextsManager = CreateDefaultSubobject<UContextsManager>(TEXT("ContextsManager"));
	MenuManager = CreateDefaultSubobject<UMenuManager>(TEXT("MenuManager"));
	Saver = CreateDefaultSubobject<USaver>(TEXT("Saver"));
}
