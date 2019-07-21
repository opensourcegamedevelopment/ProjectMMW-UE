// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectMMWGameMode.h"
#include "ProjectMMWCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectMMWGameMode::AProjectMMWGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
