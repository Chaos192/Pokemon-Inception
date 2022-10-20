// Copyright Epic Games, Inc. All Rights Reserved.

#include "PokemonInceptionGameMode.h"
#include "PokemonInceptionCharacter.h"
#include "UObject/ConstructorHelpers.h"

APokemonInceptionGameMode::APokemonInceptionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
