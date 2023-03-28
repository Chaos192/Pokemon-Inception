// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/HUD/BattleHUD.h"
#include "../Player/BattleController.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Pokemon/StaticOverworldPokemon.h"
#include "../Pokemon/AttackMoveStruct.h"
#include "../Pokemon/StatusMoveStruct.h"
#include "../Pokemon/TypeStruct.h"
#include "../Items/BallBaseStruct.h"
#include "../Items/EtherBaseStruct.h"
#include "../Items/PotionBaseStruct.h"
#include "../Items/ReviveBaseStruct.h"
#include "../SaveGame/PlayerSaveData.h"
#include "NiagaraFunctionLibrary.h"


void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("PlayerSaveSlot", 0)) {
		SaveData = Cast<UPlayerSaveData>(UGameplayStatics::LoadGameFromSlot("PlayerSaveSlot", 0));

		PlayerController->PlayerName = SaveData->PlayerName;
		PlayerController->Inventory = SaveData->InventoryData;
		PlayerController->Pokedex = SaveData->PokedexData;
		PlayerController->Money = SaveData->MoneyData;
		PlayerController->PokemonParty = SaveData->PartyData;
		PlayerController->PokemonStorage = SaveData->StorageData;
		OpponentTeam = SaveData->OpponentData;
		bIsOpponentTrainer = SaveData->bIsOpponentTrainer;
	}

	TArray<AActor*> CamerasInLevel;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraClass, CamerasInLevel);

	for (AActor* Actor : CamerasInLevel) {
		ACameraActor* CameraActor = Cast<ACameraActor>(Actor);

		if (CameraActor->ActorHasTag(FName("Scene"))) {
			SceneCamera = CameraActor;
			continue;
		}
		if (CameraActor->ActorHasTag(FName("Player"))) {
			PlayerCamera = CameraActor;
			continue;
		}
		if (CameraActor->ActorHasTag(FName("Opponent"))) {
			OpponentCamera = CameraActor;
			continue;
		}
		if (CameraActor->ActorHasTag(FName("Trainer"))) {
			TrainerCamera = CameraActor;
			continue;
		}
	}

	ResetCamera();
	BattleStart();
}

void ABattleGameMode::PlacePlayerPokemon()
{
	if ((PlayerPokemonActor) != nullptr) {
		PlayerPokemonActor->Destroy();
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	FRotator Rotation;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.PokemonActor, Position, Rotation);

	SwitchToPlayerCamera(0.5);
	Hud->ShowText("You sent out " + PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + "!");
}

void ABattleGameMode::PlaceOpponentPokemon()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	OpponentPokemonId = GetCurrentOpponent();

	FRotator Rotation = FRotator(0, 180, 0);
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[OpponentPokemonId].SpeciesData.PokemonActor, Position, Rotation);

	FString OpponentMessage;

	if (bIsOpponentTrainer) {
		OpponentMessage = "Trainer sent out " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + "!";
	}
	else {
		OpponentMessage = "A wild " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " appeared!";
	}

	SwitchToOpponentCamera(0.5);
	Hud->ShowText(OpponentMessage);
}

void ABattleGameMode::PlaceOpponentTrainer()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FRotator Rotation = FRotator(0, 270, 0);
	FVector Position = FVector(-340, 800, 190);

	TrainerActor = GetWorld()->SpawnActor<ATrainer>(TrainerClass, Position, Rotation);

	SwitchToTrainerCamera(0.5);
	Hud->ShowText("Trainer wants to battle!");
}

bool ABattleGameMode::bIsOpponentDefeated()
{
	for (FPokemonStruct Pokemon : OpponentTeam) {
		if (Pokemon.bIsFainted == false) {
			return false;
		}
	}
	return true;
}

int ABattleGameMode::GetCurrentOpponent()
{
	for (int i = 0; i < OpponentTeam.Num(); i++) {
		if (OpponentTeam[i].bIsFainted == false) {
			return i;
		}
	}

	return -1;
}

void ABattleGameMode::UseMove(int MoveId, EBattler MoveCaster)
{
	if (bHasTurnEnded == true || bHasBattleEnded == true || bDoesPlayerHaveToSwitch == true) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	FTimerDelegate MoveMessageDelegate;
	FString MoveMessage;

	FPokemonStruct Attacker;
	FPokemonStruct Opponent;

	EBattler Target = EBattler::Player;;

	bool bIsStruggling = false;

	if (MoveCaster == EBattler::Player) {
		Attacker = PlayerController->PokemonParty[PlayerPokemonId];
		Opponent = OpponentTeam[OpponentPokemonId];
	}

	else if (MoveCaster == EBattler::Opponent) {
		Attacker = OpponentTeam[OpponentPokemonId];
		Opponent = PlayerController->PokemonParty[PlayerPokemonId];
	}

	FMoveBaseStruct* Move;
	if (MoveId == -1) {
		 Move = AttackMovesDT->FindRow<FAttackMoveStruct>(FName(*FString("Struggle")), "");
	}
	else {
		Move = &Attacker.Moves[MoveId];
	}

	if (Move->MoveStructType == "Attack") {
		FAttackMoveStruct* AttackMove;
		
		if (MoveId != -1) {
			AttackMove = AttackMovesDT->FindRow<FAttackMoveStruct>(Attacker.Moves[MoveId].MoveID, "");
		}
		else {
			AttackMove = AttackMovesDT->FindRow<FAttackMoveStruct>(FName(*FString("Struggle")), "");
			bIsStruggling = true;
		}

		//Base damage
		int Damage = (((100 + Attacker.Attack + 15 * Attacker.Level) * AttackMove->BaseDamage) / (Opponent.Defence + 50)) / 5;

		float EffectMultiplier = 0;

		//Adding effect multiplier
		if (Attacker.Effects.Contains(EEffect::AttackUp)) {
			EffectMultiplier = 1.5;
		}
		else if (Attacker.Effects.Contains(EEffect::AttackDown)) {
			EffectMultiplier = 0.5;
		}
		else EffectMultiplier = 1;

		if (Opponent.Effects.Contains(EEffect::DefenceUp)) {
			EffectMultiplier /= 1.5;
		}
		else if (Opponent.Effects.Contains(EEffect::DefenceDown)) {
			EffectMultiplier /= 0.5;
		}

		Damage *= EffectMultiplier;

		//Is it a critical hit
		int CriticalHitChance = FMath::RandRange(1, 100);
		if (CriticalHitChance <= 5) {
			Damage *= 1.5;
		}

		//Multiplying by a random value
		float RandomMultiplier = FMath::RandRange(0.85, 1.00);
		Damage *= RandomMultiplier;

		//Same type attack bonus - STAB
		if ((Attacker.SpeciesData.Type1 == AttackMove->MoveType) || (Attacker.SpeciesData.Type2 == AttackMove->MoveType)) {
			Damage *= 1.25;
		}

		//Move effectiveness 
		if (MoveId != -1) {
			FTypeStruct* MovetTypeStruct = TypesDT->FindRow<FTypeStruct>(FName(FTypeStruct::ToString(AttackMove->MoveType)), "");

			if (MovetTypeStruct->SuperEffectiveAgainst.Contains(Opponent.SpeciesData.Type1)) {
				Damage *= 2;
			}
			else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(Opponent.SpeciesData.Type1)) {
				Damage *= 0.5;
			}
			else if (MovetTypeStruct->NoEffectAgainst.Contains(Opponent.SpeciesData.Type1)) {
				Damage *= 0;
			}

			if (Opponent.SpeciesData.Type2 != ETypes::None) {
				if (MovetTypeStruct->SuperEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
					Damage *= 2;
				}
				else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
					Damage *= 0.5;
				}
				else if (MovetTypeStruct->NoEffectAgainst.Contains(Opponent.SpeciesData.Type2)) {
					Damage *= 0;
				}
			}
		}

		if (MoveCaster == EBattler::Player) {
			OpponentTeam[OpponentPokemonId].RecieveDamage(Damage);
			
			if (MoveId == -1) {
				PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(PlayerController->PokemonParty[PlayerPokemonId].MaxHP/4);
			}
			else {
				PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
			}

			Target = EBattler::Opponent;
		}

		else if (MoveCaster == EBattler::Opponent) {
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Damage);

			if (MoveId == -1) {
				OpponentTeam[OpponentPokemonId].RecieveDamage(OpponentTeam[OpponentPokemonId].MaxHP/4);
			}
			else {
				OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
			}
			
			Target = EBattler::Player;
		}

		if (bIsStruggling) {
			MoveMessage = Attacker.SpeciesData.Name.ToString() + " used Struggle and dealt " + FString::FromInt(Damage) + " damage, but also hurt itself badly!";
		}
		else {
			MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + AttackMove->Name.ToString() + " and dealt " + FString::FromInt(Damage) + " damage!";
		}
	}

	
	if (Move->MoveStructType == "Status") {
		FStatusMoveStruct* StatusMove = StatusMovesDT->FindRow<FStatusMoveStruct>(Attacker.Moves[MoveId].MoveID, "");

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + ", but it failed!";
		bool bIsMoveSuccessful = false;

		for (EEffect Effect : StatusMove->MoveEffects) {
			switch (StatusMove->Target) {
				case ETarget::Self:
					if (Attacker.AddEffect(Effect) == true) bIsMoveSuccessful = true;
				
					break;

				case ETarget::Opponent:
					if (Opponent.AddEffect(Effect) == true) bIsMoveSuccessful = true;
			
					break;
			}
		}

		if(bIsMoveSuccessful == true) MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + "!";
		
		if (MoveCaster == EBattler::Player) {
			PlayerController->PokemonParty[PlayerPokemonId] = Attacker;
			OpponentTeam[OpponentPokemonId] = Opponent;

			switch (StatusMove->Target) {
				case ETarget::Self:
					Target = EBattler::Player;
					break;
				case ETarget::Opponent:
					Target = EBattler::Opponent;
					break;
			}

			PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
		}

		else if (MoveCaster == EBattler::Opponent) {
			PlayerController->PokemonParty[PlayerPokemonId] = Opponent;
			OpponentTeam[OpponentPokemonId] = Attacker;

			switch (StatusMove->Target) {
				case ETarget::Opponent:
					Target = EBattler::Player;
					break;
				case ETarget::Self:
					Target = EBattler::Opponent;
					break;
			}

			OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
		}
	}

	if (MoveCaster == EBattler::Opponent) {
		MoveMessage = "Opponent " + MoveMessage;
	}
	
	MoveOutcome(MoveMessage, MoveCaster, Target);
}

void ABattleGameMode::MoveOutcome(FString MoveMessage, EBattler MoveCaster, EBattler Target)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FTimerHandle CameraSwitchTimer;
	FTimerHandle MoveSplashTimer;
	FTimerHandle MoveMessageTimer;
	FTimerHandle PlayerFaintTimer;
	FTimerHandle OpponentFaintTimer;
	FTimerHandle BattleEndTimer;
	FTimerHandle OpponentSwitchTimer;
	FTimerHandle TurnEndTimer;

	FTimerDelegate MoveMessageDelegate;
	FTimerDelegate CameraTimerDelegate;
	FTimerDelegate MoveSplashDelegate;

	if (CurrentBattleTime == 0) {
		Hud->ShowText(MoveMessage);

		if (Target == EBattler::Player) {
			SwitchToPlayerCamera(0.5);
		}
		if (Target == EBattler::Opponent) {
			SwitchToOpponentCamera(0.5);
		}

		ShowMoveSplash(MoveCaster, Target);
	}
	else {
		MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);
		GetWorldTimerManager().SetTimer(MoveMessageTimer, MoveMessageDelegate, CurrentBattleTime, false);

		if (Target == EBattler::Player) {
			CameraTimerDelegate.BindUFunction(this, FName("SwitchToPlayerCamera"), float(0.5));
			GetWorldTimerManager().SetTimer(CameraSwitchTimer, CameraTimerDelegate, CurrentBattleTime, false);
		}
		if (Target == EBattler::Opponent) {
			CameraTimerDelegate.BindUFunction(this, FName("SwitchToOpponentCamera"), float(0.5));
			GetWorldTimerManager().SetTimer(CameraSwitchTimer, CameraTimerDelegate, CurrentBattleTime, false);
		}

		MoveSplashDelegate.BindUFunction(this, FName("ShowMoveSplash"), MoveCaster, Target);
		GetWorldTimerManager().SetTimer(MoveSplashTimer, MoveSplashDelegate, CurrentBattleTime, false);
	}
	CurrentBattleTime += 1.5;

	if (PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == true) {
		GetWorldTimerManager().SetTimer(PlayerFaintTimer, this, &ABattleGameMode::PlayerFaints, CurrentBattleTime, false);
		CurrentBattleTime += 1.5;
		bHasTurnEnded = true;

		if (PlayerController->bIsPartyDefeated() == true) {
			GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentBattleTime, false);
			bHasBattleEnded = true;
			bIsBattleVictory = false;
			
		}
		else {
			bDoesPlayerHaveToSwitch = true;
		}
	}

	if (OpponentTeam[OpponentPokemonId].bIsFainted == true && PlayerController->bIsPartyDefeated() == false) {
		GetWorldTimerManager().SetTimer(OpponentFaintTimer, this, &ABattleGameMode::OpponentFaints, CurrentBattleTime, false);
		CurrentBattleTime += 1.5;
		bHasTurnEnded = true;

		if (bIsOpponentDefeated() == true) {
			GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentBattleTime, false);
			bHasBattleEnded = true;
			bIsBattleVictory = true;
			
		}
		else {
			FTimerDelegate SwitchOpponent;
			SwitchOpponent.BindUFunction(this, FName("PlaceOpponentPokemon"), OpponentPokemonId);

			GetWorldTimerManager().SetTimer(OpponentSwitchTimer, SwitchOpponent, CurrentBattleTime, false);
			CurrentBattleTime += 1.5;
		}
	}
}

void ABattleGameMode::UseItem()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FString ItemMessage = "You used a " + PlayerController->Inventory[SelectedItemID].Name.ToString() + ", ";

	if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Potion") {
		if (PlayerController->PokemonParty[SelectedPokemonID].bIsFainted == true || PlayerController->PokemonParty[SelectedPokemonID].bIsFullHp() == true) {
			ItemMessage += "but it failed!";
		}

		else {
			FPotionBaseStruct* Potion = PotionsDT->FindRow<FPotionBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RestoreHP(Potion->RestoredHP);

			PlayerController->Inventory.RemoveAt(SelectedItemID);

			ItemMessage += PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() + "'s HP was restored!";
		}
	}

	else if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Revive") {
		if (PlayerController->PokemonParty[SelectedPokemonID].bIsFainted == false) {
			ItemMessage += "but it failed!";
		}

		else {
			FReviveBaseStruct* Revive = RevivesDT->FindRow<FReviveBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RecoverStatus();
			PlayerController->PokemonParty[SelectedPokemonID].RestoreHP(PlayerController->PokemonParty[SelectedPokemonID].MaxHP * Revive->RevivePercent);

			PlayerController->Inventory.RemoveAt(SelectedItemID);

			ItemMessage += PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() + " was revived!";
		}
	}

	else if (PlayerController->Inventory[SelectedItemID].ItemStructType == "Ether") {
		if ( PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].bHasMaxPP() == true) {
			ItemMessage += "but it failed!";
		}

		else {
			FEtherBaseStruct* Ether = EthersDT->FindRow<FEtherBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");
			PlayerController->PokemonParty[SelectedPokemonID].RestorePP(Ether->RestoredPP, SelectedMoveID);

			PlayerController->Inventory.RemoveAt(SelectedItemID);
			ItemMessage += "restored PP to " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() +
				"'s move " + PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].Name.ToString() + "!";
		}
	}

	Hud->ShowText(ItemMessage);

	bHasSelectedItem = false;
	bHasSelectedEther = false;
	CurrentBattleTime += 1.5;
}

void ABattleGameMode::UseBall()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FTimerHandle CatchTimer;
	FTimerHandle BattleEndTimer;

	FBallBaseStruct* Ball = BallsDT->FindRow<FBallBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");

	OpponentPokemonActor->Destroy();

	FRotator Rotation = FRotator(0, 180, 0);
	FVector Position = FVector(-350, 440, 120);

	ThrownBallActor = GetWorld()->SpawnActor<ABallActor>(Ball->BallActor, Position, Rotation);

	SwitchToOpponentCamera(0.5);
	FString BallMessage = "You threw a " + PlayerController->Inventory[SelectedItemID].Name.ToString() + "...";
	Hud->ShowText(BallMessage);
	CurrentBattleTime += 1.5;

	PlayerController->Inventory.RemoveAt(SelectedItemID);

	FPokemonStruct OpponentData = OpponentTeam[OpponentPokemonId];

	float CatchRate = ((OpponentData.MaxHP * 3 - OpponentData.CurrHP * 2) * OpponentData.SpeciesData.CatchRate * Ball->CatchMultiplier) / (OpponentData.MaxHP * 3);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "Catch rate: " + FString::SanitizeFloat(CatchRate) + "%");

	int RandomPercent = FMath::FMath::RandRange(1, 100);

	if (CatchRate > RandomPercent) {
		bHasBattleEnded = true;
		bIsBattleVictory = true;

		GetWorldTimerManager().SetTimer(CatchTimer, this, &ABattleGameMode::CatchSuccess, CurrentBattleTime, false);
		CurrentBattleTime += 1.5;
		GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentBattleTime, false);

		return;
	}
	else {
		GetWorldTimerManager().SetTimer(CatchTimer, this, &ABattleGameMode::CatchFail, CurrentBattleTime, false);
		CurrentBattleTime += 1.5;

		return;
	}
}

void ABattleGameMode::CatchSuccess()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	PlayerController->ObtainPokemon(OpponentTeam[OpponentPokemonId]);
	Hud->ShowText("You caught " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + "!");
}

void ABattleGameMode::CatchFail()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ThrownBallActor->Destroy();

	FRotator Rotation = FRotator(0, 180, 0);
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[OpponentPokemonId].SpeciesData.PokemonActor, Position, Rotation);
	Hud->ShowText(OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " got out!");
}

void ABattleGameMode::OpponentFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	SwitchToOpponentCamera(0);

	OpponentPokemonActor->Destroy();

	int Exp = OpponentTeam[OpponentPokemonId].Level * OpponentTeam[OpponentPokemonId].SpeciesData.BaseExp / 7;

	if (bIsOpponentTrainer) {
		Exp *= 1.5;
	}

	FString DefeatMessage = "The opponent " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " was defeated! ";

	if(!PlayerController->PokemonParty[PlayerPokemonId].bIsFainted){
		if (PlayerController->PokemonParty[PlayerPokemonId].GainExp(Exp) == true) {
			PlayerController->PokemonParty[PlayerPokemonId].CheckForNewMoves();
		}

		DefeatMessage += PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " recieved " + FString::FromInt(Exp) + " Exp!";
	}

	Hud->ShowText(DefeatMessage);
}

void ABattleGameMode::PlayerFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	SwitchToPlayerCamera(0);

	PlayerPokemonActor->Destroy();

	Hud->ShowText(PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " was defeated!");
}

int ABattleGameMode::SelectOpponentMove()
{
	if (OpponentTeam[OpponentPokemonId].bHasRanOutOfPP() == true) {
		return -1;
	}

	int OpponentMoveID = FMath::RandRange(0, OpponentTeam[OpponentPokemonId].CurrentMoves.Num() - 1);

	if (OpponentTeam[OpponentPokemonId].Moves[OpponentTeam[OpponentPokemonId].CurrentMoves[OpponentMoveID]].CurrPowerPoints == 0) {
		OpponentMoveID = SelectOpponentMove();
	}

	return OpponentTeam[OpponentPokemonId].CurrentMoves[OpponentMoveID];
}

void ABattleGameMode::BattleStart()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	CurrentBattleTime = 0;

	FTimerHandle TrainerTimer;
	FTimerHandle SendOutOpponentTimer;
	FTimerHandle SendOutPlayerTimer;
	FTimerHandle StartTimer;

	PlayerPokemonId = PlayerController->GetLeadPokemon();

	if (bIsOpponentTrainer) {
		GetWorldTimerManager().SetTimer(TrainerTimer, this, &ABattleGameMode::PlaceOpponentTrainer, 0.0001, false);
		CurrentBattleTime += 1.5;
	}

	if (CurrentBattleTime == 0) {
		GetWorldTimerManager().SetTimer(SendOutOpponentTimer, this, &ABattleGameMode::PlaceOpponentPokemon, 0.0001, false);
		CurrentBattleTime += 1.5;
	}
	else {
		GetWorldTimerManager().SetTimer(SendOutOpponentTimer, this, &ABattleGameMode::PlaceOpponentPokemon, CurrentBattleTime, false);
		CurrentBattleTime += 1.5;
	}

	GetWorldTimerManager().SetTimer(SendOutPlayerTimer, this, &ABattleGameMode::PlacePlayerPokemon, CurrentBattleTime, false);
	CurrentBattleTime += 1.5;
	GetWorldTimerManager().SetTimer(StartTimer, Hud, &ABattleHUD::ShowBattleStartWidget, CurrentBattleTime, false);
}

void ABattleGameMode::BattleTurn(EAction PlayerAction)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	CurrentBattleTime = 0;
	bHasTurnEnded = false;
	OpponentSelectedMoveID = SelectOpponentMove();

	if (PlayerAction == EAction::UseMove) {
		if (PlayerController->PokemonParty[PlayerPokemonId].GetSpeed() > OpponentTeam[OpponentPokemonId].GetSpeed()) {
			UseMove(SelectedMoveID, EBattler::Player);
			UseMove(OpponentSelectedMoveID, EBattler::Opponent);
			EndTurn();
		}

		else {
			UseMove(OpponentSelectedMoveID, EBattler::Opponent);
			UseMove(SelectedMoveID, EBattler::Player);
			EndTurn();
		}	
	}

	if (PlayerAction == EAction::SwitchOut) {
		PlayerController->PokemonParty[PlayerPokemonId].ClearEffects();
		PlayerPokemonId = SelectedPokemonID;
		PlacePlayerPokemon();
		CurrentBattleTime += 1.5;

		if (!bDoesPlayerHaveToSwitch) {
			UseMove(OpponentSelectedMoveID, EBattler::Opponent);
		}
		EndTurn();
		bDoesPlayerHaveToSwitch = false;
	}

	if (PlayerAction == EAction::UseHealingItem) {
		UseItem();
		UseMove(OpponentSelectedMoveID, EBattler::Opponent);
		EndTurn();
	}

	if (PlayerAction == EAction::UsePokeball) {
		UseBall();
		UseMove(OpponentSelectedMoveID, EBattler::Opponent);
		EndTurn();
	}
}

void ABattleGameMode::EndTurn()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	FTimerHandle EndTurnTimer;

	//SelectedItemID = 0;
	//SelectedMoveID = 0;
	//SelectedPokemonID = 0;

	if (bDoesPlayerHaveToSwitch == true && bHasBattleEnded == false) {
		GetWorldTimerManager().SetTimer(EndTurnTimer, Hud, &ABattleHUD::ShowPokemon, CurrentBattleTime, false);
	}

	if (bHasBattleEnded == false && PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == false) {
		GetWorldTimerManager().SetTimer(EndTurnTimer, Hud, &ABattleHUD::ShowBattleStartWidget, CurrentBattleTime, false);
	}
}

void ABattleGameMode::BattleEnd()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	FTimerHandle ExitTimer;

	ResetCamera();

	int Money = 0;

	for (FPokemonStruct Pokemon : OpponentTeam) {
		Money += Pokemon.Level * 50;
	}
	if (bIsOpponentTrainer) {
		Money *= 2;
	}

	if (bIsBattleVictory == true) {
		if ((PlayerController->Money + Money) > 1000000) {
			Money = 1000000 - PlayerController->Money;
		}

		PlayerController->Money += Money;

		Hud->ShowText("You won! You recieved " + FString::FromInt(Money) + " money!");
	}

	else if (bIsBattleVictory == false) {
		if ((PlayerController->Money - Money) < 0) {
			Money = 0 + PlayerController->Money;
		}

		PlayerController->Money -= Money;

		Hud->ShowText("You don't have any Pokemon that can fight! You lost " + FString::FromInt(Money) + " money!");
	}

	GetWorldTimerManager().SetTimer(ExitTimer, this, &ABattleGameMode::ExitBattleMap, CurrentBattleTime, false);
}

void ABattleGameMode::ResetCamera()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	PlayerController->SetViewTargetWithBlend(SceneCamera, 0, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::SwitchToPlayerCamera(float BlendTime)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	PlayerController->SetViewTargetWithBlend(PlayerCamera, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::SwitchToOpponentCamera(float BlendTime)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	PlayerController->SetViewTargetWithBlend(OpponentCamera, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::SwitchToTrainerCamera(float BlendTime)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	PlayerController->SetViewTargetWithBlend(TrainerCamera, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::ShowMoveSplash(EBattler MoveCaster, EBattler Target)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AActor* PokemonTarget = nullptr;

	if (Target == EBattler::Player) {
		PokemonTarget = PlayerPokemonActor;
	}
	else if (Target == EBattler::Opponent) {
		PokemonTarget = OpponentPokemonActor;
	}

	if (MoveCaster == EBattler::Player) {
		FMoveBaseStruct Move = PlayerController->PokemonParty[PlayerPokemonId].Moves[SelectedMoveID];

		if (Move.Particle) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Move.Particle, PokemonTarget->GetActorLocation());
		}

		if (Move.Sound) {
			UGameplayStatics::PlaySound2D(GetWorld(), Move.Sound);
		}

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Your Pokemon used " + Move.Name.ToString());
	}
			
	if (MoveCaster == EBattler::Opponent) {
		FMoveBaseStruct Move = OpponentTeam[OpponentPokemonId].Moves[OpponentSelectedMoveID];

		if (Move.Particle) {
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Move.Particle, PokemonTarget->GetActorLocation());
		}

		if (Move.Sound) {
			UGameplayStatics::PlaySound2D(GetWorld(), Move.Sound);
		}

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Opponent Pokemon used " + Move.Name.ToString());
	}
}

void ABattleGameMode::SelectMove(int MoveId)
{
	SelectedMoveID = MoveId;
	BattleTurn(EAction::UseMove);
}

void ABattleGameMode::SelectPokemon(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	if (PlayerPokemonId == InId || PlayerController->PokemonParty[InId].bIsFainted == true) {
		return;
	}

	SelectedPokemonID = InId;
	BattleTurn(EAction::SwitchOut);
}

void ABattleGameMode::SelectItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (PlayerController->Inventory[InId].bUsableInBattle == false) {
		return;
	}

	if (PlayerController->Inventory[InId].ItemStructType == "Ball") {
		FTimerHandle ShowTurnStartTimer;

		if (!PlayerController->bCanObtainMorePokemon()) {
			Hud->ShowText("You can't catch more pokemon!");
			GetWorldTimerManager().SetTimer(ShowTurnStartTimer, Hud, &ABattleHUD::ShowBattleStartWidget, 1.5, false);
		}

		else if (bIsOpponentTrainer) {
			Hud->ShowText("You can't catch other people's pokemon!");
			GetWorldTimerManager().SetTimer(ShowTurnStartTimer, Hud, &ABattleHUD::ShowBattleStartWidget, 1.5, false);
		}

		else {
			SelectedItemID = InId;
			BattleTurn(EAction::UsePokeball);
		}
	}

	else {
		if(PlayerController->Inventory[InId].ItemStructType == "Ether") {
			bHasSelectedEther = true;
		}

		else {
			bHasSelectedItem = true;
		}

		SelectedItemID = InId;
		Hud->ShowPokemon();
	}
}

void ABattleGameMode::SelectPokemonToUseItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	if (Hud->BIsMovePopupInViewport()) {
		return;
	}

	SelectedPokemonID = InId;

	if (bHasSelectedItem) {
		BattleTurn(EAction::UseHealingItem);
	}
}

void ABattleGameMode::SelectMoveToUseItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	SelectedMoveID = InId;
	BattleTurn(EAction::UseHealingItem);
}

void ABattleGameMode::ShowPokemonInMenu()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	TArray<FPokemonStruct> Party = PlayerController->PokemonParty;

	for (int i = 0; i < Party.Num(); i++) {
		UPokemonSlotWidget* PokemonSlotWidget = CreateWidget<UPokemonSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetPokemonSlotWidgetClass());

		PokemonSlotWidget->SetPokemonName(Party[i].SpeciesData.Name);
		PokemonSlotWidget->SetPokemonLevel(Party[i].Level);
		PokemonSlotWidget->SetPokemonImage(Party[i].SpeciesData.Image);
		PokemonSlotWidget->SetPokemonHP(Party[i].CurrHP, Party[i].MaxHP);
		PokemonSlotWidget->SetPokemonEXP(Party[i].CurrExp, Party[i].RequiredExp);
		PokemonSlotWidget->SetPokemon(i);

		PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowPokemonSummary);
		if (bHasSelectedItem) {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowUseItemPopup);
		}
		else if (bHasSelectedEther) {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowMoveSelectionPopup);
			PokemonSlotWidget->PokemonClick.AddDynamic(this, &ABattleGameMode::SelectPokemonToUseItem);
		}
		else {
			PokemonSlotWidget->PokemonClick.AddDynamic(Hud, &ABattleHUD::ShowSwitchOutPopup);
		}

		PokemonSlotDelegate.Broadcast(PokemonSlotWidget);
	}
}

void ABattleGameMode::FillBagWidget()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	TArray<FItemBaseStruct> UniqueItems;
	TArray<int> ItemCount;

	for (FItemBaseStruct Item : PlayerController->Inventory) {
		if (UniqueItems.Contains(Item) == false) {
			UniqueItems.Add(Item);
		}
	}

	for (FItemBaseStruct Item : UniqueItems) {
		int Count = 0;

		for (FItemBaseStruct ItemToSearch : PlayerController->Inventory) {
			if (Item == ItemToSearch) {
				Count++;
			}
		}

		ItemCount.Add(Count);
	}

	for (int i = 0; i < UniqueItems.Num(); i++) {
		UItemSlotWidget* ItemSlotWidget = CreateWidget<UItemSlotWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetItemSlotWidgetClass());

		ItemSlotWidget->SetItemName(UniqueItems[i].Name);
		ItemSlotWidget->SetItemImage(UniqueItems[i].Image);
		ItemSlotWidget->SetItemCount(ItemCount[i]);
		ItemSlotWidget->SetItemID(PlayerController->Inventory.Find(UniqueItems[i]));

		ItemSlotWidget->ItemClicked.AddDynamic(Hud, &ABattleHUD::ShowItemInfo);

		ItemSlotDelegate.Broadcast(ItemSlotWidget);
	}
}

void ABattleGameMode::ShowPokemonMoves()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FPokemonStruct Attacker = PlayerController->PokemonParty[PlayerPokemonId];

	for (int i = 0; i < Attacker.CurrentMoves.Num(); i++) {
		
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Attacker.Moves[Attacker.CurrentMoves[i]].Name, Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints,
			Attacker.Moves[Attacker.CurrentMoves[i]].PowerPoints, Attacker.Moves[Attacker.CurrentMoves[i]].MoveType);
		MoveButton->SetMove(Attacker.CurrentMoves[i]);

		if (Attacker.Moves[Attacker.CurrentMoves[i]].MoveStructType == "Attack") {
			MoveButton->SetEffectiveness(GetMoveEffectiveness(Attacker.Moves[Attacker.CurrentMoves[i]].MoveType));
		}

		if (Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints > 0) {
			MoveButton->ButtonClicked.AddDynamic(this, &ABattleGameMode::SelectMove);
		}
		MoveButton->ButtonHovered.AddDynamic(Hud, &ABattleHUD::ShowMoveInfo);
		MoveButton->ButtonUnHovered.AddDynamic(Hud, &ABattleHUD::ClearPopup);

		MoveDelegate.Broadcast(MoveButton);
	}
}

FString ABattleGameMode::GetMoveEffectiveness(ETypes MoveType)
{
	FTypeStruct* MovetTypeStruct = TypesDT->FindRow<FTypeStruct>(FName(FTypeStruct::ToString(MoveType)), "");
	FPokemonBaseStruct OpponentData = OpponentTeam[OpponentPokemonId].SpeciesData;

	float DamageMultiplier = 1;

	if (MovetTypeStruct->SuperEffectiveAgainst.Contains(OpponentData.Type1)) {
		DamageMultiplier *= 2;
	}
	else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(OpponentData.Type1)) {
		DamageMultiplier *= 0.5;
	}
	else if (MovetTypeStruct->NoEffectAgainst.Contains(OpponentData.Type1)) {
		DamageMultiplier *= 0;
	}
	
	if (OpponentTeam[OpponentPokemonId].SpeciesData.Type2 != ETypes::None) {
		if (MovetTypeStruct->SuperEffectiveAgainst.Contains(OpponentData.Type2)) {
			DamageMultiplier *= 2;
		}
		else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(OpponentData.Type2)) {
			DamageMultiplier *= 0.5;
		}
		else if (MovetTypeStruct->NoEffectAgainst.Contains(OpponentData.Type2)) {
			DamageMultiplier *= 0;
		}
	}
	
	if (DamageMultiplier < 1 && DamageMultiplier > 0) {
		return "Not Very Effective";
	}

	if (DamageMultiplier > 1) {
		return "Super Effective";
	}
	
	if (DamageMultiplier == 0) {
		return "No Effect";
	}

	return "Effective";
}

FPokemonStruct ABattleGameMode::GetCurrentOpponentStruct() {
	return OpponentTeam[OpponentPokemonId];
}

int ABattleGameMode::GetPlayerPokemonId() {
	return PlayerPokemonId;
}

bool ABattleGameMode::HasPlayerRanOutOfPP()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return false;
	}

	return PlayerController->PokemonParty[PlayerPokemonId].bHasRanOutOfPP();
}

bool ABattleGameMode::bHasToSwitchPokemon()
{
	return bDoesPlayerHaveToSwitch;
}

void ABattleGameMode::ExitBattleMap()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	for (int i = 0; i < PlayerController->PokemonParty.Num(); i++) {
		PlayerController->PokemonParty[i].ClearEffects();
	}

	UPlayerSaveData* SaveData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));

	SaveData->PlayerName = PlayerController->PlayerName;
	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;

	UGameplayStatics::SaveGameToSlot(SaveData, "PlayerSaveSlot", 0);

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
}

void ABattleGameMode::Run()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FTimerHandle RunTimer;

	if (bIsOpponentTrainer) {
		Hud->ShowText("You can't run away from a trainer battle!");
		GetWorldTimerManager().SetTimer(RunTimer, Hud, &ABattleHUD::ShowBattleStartWidget, 2, false);
	}
	else {
		Hud->ShowText("Got away safely...");
		GetWorldTimerManager().SetTimer(RunTimer, this, &ABattleGameMode::ExitBattleMap, 2, false);
	}
}

