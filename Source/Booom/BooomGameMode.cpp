// Copyright Epic Games, Inc. All Rights Reserved.

#include "BooomGameMode.h"
#include "BooomCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABooomGameMode::ABooomGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
