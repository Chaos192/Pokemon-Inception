// Fill out your copyright notice in the Description page of Project Settings.


#include "Trainer.h"
#include "../../Player/PlayerCharacterController.h"
#include "../../GameModes/OverworldGameMode.h"
#include "../../UI/HUD/OverworldHUD.h"


ATrainer::ATrainer()
{
	Name = "Trainer";
}

void ATrainer::BeginPlay()
{
	Super::BeginPlay();

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	for (int i = 0; i < TrainerTeamData.Num(); i++) {
		FPokemonStruct AddedPokemon;
		TArray<FName> Moveset;
		Moveset.Add(TrainerTeamData[i].Move1);
		Moveset.Add(TrainerTeamData[i].Move2);
		Moveset.Add(TrainerTeamData[i].Move3);
		Moveset.Add(TrainerTeamData[i].Move4);

		FPokemonBaseStruct* PokemonSpecies = GameMode->PokemonDT->FindRow<FPokemonBaseStruct>(TrainerTeamData[i].PokemonID, "");

		AddedPokemon.Init(TrainerTeamData[i].PokemonLevel, *PokemonSpecies);
		AddedPokemon.InitMovesCustom(Moveset, GameMode->GetMoveDT());

		TrainerTeam.Add(AddedPokemon);
	}
}

void ATrainer::Throw()
{
	if (ThrowAnimMontage) {
		PlayAnimMontage(ThrowAnimMontage, 1);
	}
}

void ATrainer::Interact(APlayerController* PlayerController)
{
	APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(PlayerController);
	if (!IsValid(PlayerCharacterController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerCharacterController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PlayerCharacterController->TrainersDefeated < DefeatedTrainersRequiredToChallange) {
		Hud->OnScreenMessage("You aren't strong enough to challange this trainer!");
		return;
	}

	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	FTimerHandle BattleStartTimer;
	FTimerDelegate BattleStartDelegate;

	GameMode->PauseGame(EPause::Pause);
	GameMode->SaveGame();
	GameMode->SaveLevelData(nullptr);
	GameMode->PlayEncounterSequence();
	Hud->Clear();

	BattleStartDelegate.BindUFunction(GameMode, FName("InitiateBattle"), TrainerTeam, true, bHasBeenDefeated);
	GetWorldTimerManager().SetTimer(BattleStartTimer, BattleStartDelegate, 1.5, false);
}
