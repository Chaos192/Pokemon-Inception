// Fill out your copyright notice in the Description page of Project Settings.


#include "Trainer.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"


ATrainer::ATrainer()
{
	Name = "Trainer";
}

void ATrainer::Throw()
{
	if (ThrowAnimMontage) {
		PlayAnimMontage(ThrowAnimMontage, 1);
	}
}

void ATrainer::Interact(APlayerController* PlayerController)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode == nullptr) {
		return;
	}

	TArray<FPokemonStruct> TrainerTeam;

	for (int i = 0; i < PokemonIDs.Num(); i++) {
		FPokemonBaseStruct* PokemonSpecies = GameMode->PokemonDT->FindRow<FPokemonBaseStruct>(PokemonIDs[i], "");
		FPokemonStruct AddedPokemon;
		AddedPokemon.Init(PokemonLevels[i], *PokemonSpecies);
		AddedPokemon.InitMoves(GameMode->GetMoveDT());

		TrainerTeam.Add(AddedPokemon);
	}

	GameMode->SaveGame();
	GameMode->SaveLevelData(nullptr);
	GameMode->InitiateBattle(TrainerTeam, true);
}
