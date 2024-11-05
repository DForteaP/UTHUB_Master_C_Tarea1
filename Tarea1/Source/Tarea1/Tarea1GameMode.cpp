// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tarea1GameMode.h"
#include "Tarea1Character.h"
#include "UObject/ConstructorHelpers.h"

ATarea1GameMode::ATarea1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
