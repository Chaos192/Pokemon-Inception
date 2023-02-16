// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "../UI/BattleUI/BattleHUD.h"
#include "../UI/BattleUI/MoveButtonWidget.h"
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


void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist("SaveSlot", 0)) {
		SaveData = Cast<UWorldSaveData>(UGameplayStatics::LoadGameFromSlot("SaveSlot", 0));

		PlayerController->Inventory = SaveData->InventoryData;
		PlayerController->Pokedex = SaveData->PokedexData;
		PlayerController->Money = SaveData->MoneyData;
		PlayerController->PokemonParty = SaveData->PartyData;
		PlayerController->PokemonStorage = SaveData->StorageData;
		OpponentTeam = SaveData->OpponentData;

		SavedGameMapData = SaveData->GameMapData;
	}

	TArray<AActor*> CamerasInLevel;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Camera, CamerasInLevel);

	for (AActor* Actor : CamerasInLevel) {
		ABattleCamera* CameraActor = Cast<ABattleCamera>(Actor);

		if (CameraActor->Tag == "Scene") {
			SceneCamera = CameraActor;
			continue;
		}
		if (CameraActor->Tag == "Player") {
			PlayerCamera = CameraActor;
			continue;
		}
		if (CameraActor->Tag == "Opponent") {
			OpponentCamera = CameraActor;
			continue;
		}
	}

	ResetCamera();
	BattleStart();
}

void ABattleGameMode::PlacePlayerPokemon(int PokemonId)
{
	if (IsValid(PlayerPokemonActor)) {
		PlayerPokemonActor->Destroy();
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FRotator Rotation;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, -270, 115);

	PlayerPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(PlayerController->PokemonParty[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);

	PlayerController->SetViewTargetWithBlend(PlayerCamera, 0, EViewTargetBlendFunction::VTBlend_Linear);
	Hud->ShowText("You sent out " + PlayerController->PokemonParty[PokemonId].SpeciesData.Name.ToString() + "!");
}

void ABattleGameMode::PlaceOpponentPokemon(int PokemonId)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FRotator Rotation;
	Rotation.Yaw = 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[PokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);

	FString OpponentMessage;

	if (OpponentTeam.Num() == 1) {
		OpponentMessage = "A wild " + OpponentTeam[PokemonId].SpeciesData.Name.ToString() + " appeared!";
	}
	else {
		OpponentMessage = "Trainer sent out " + OpponentTeam[PokemonId].SpeciesData.Name.ToString() + "!";
	}

	SwitchToOpponentCamera();
	Hud->ShowText(OpponentMessage);
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

void ABattleGameMode::UseMove(int MoveId, FString AttackerContextString)
{
	if (bHasTurnEnded == true || bHasBattleEnded == true || bDoesPlayerHaveToSwitch == true) {
		return;
	}

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FTimerDelegate MoveMessageDelegate;
	FString MoveMessage;
	FString CameraContext;

	FPokemonStruct Attacker;
	FPokemonStruct Opponent;

	if (AttackerContextString == "Player") {
		Attacker = PlayerController->PokemonParty[PlayerPokemonId];
		Opponent = OpponentTeam[OpponentPokemonId];
	}

	else if (AttackerContextString == "Opponent") {
		Attacker = OpponentTeam[OpponentPokemonId];
		Opponent = PlayerController->PokemonParty[PlayerPokemonId];
	}

	if (MoveId == -1) {
		int Damage = (((100 + Attacker.Attack + 15 * Attacker.Level) * 50) / (Opponent.Defence + 50)) / 5;

		if (AttackerContextString == "Player") {
			OpponentTeam[OpponentPokemonId].RecieveDamage(Damage);
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Attacker.MaxHP/4);
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Damage);
			OpponentTeam[OpponentPokemonId].RecieveDamage(Attacker.MaxHP / 4);
		}

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used Struggle and dealt " + FString::FromInt(Damage) + " damage, but injured itself in the process!";
		Hud->ShowText(MoveMessage);
		Hud->ShowOpponentPokemonStatus();
		Hud->ShowPlayerPokemonStatus();

		return;
	}

	if (Attacker.Moves[MoveId].MoveStructType == "Attack") {
		FAttackMoveStruct* AttackMove = AttackMovesDT->FindRow<FAttackMoveStruct>(Attacker.Moves[MoveId].MoveID, "");

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
		FTypeStruct* MovetTypeStruct = TypesDT->FindRow<FTypeStruct>(FName(ETypeToString(AttackMove->MoveType)), "");
		
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
			if(MovetTypeStruct->SuperEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 2;
			}
			else if (MovetTypeStruct->NotVeryEffectiveAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 0.5;
			}
			else if (MovetTypeStruct->NoEffectAgainst.Contains(Opponent.SpeciesData.Type2)) {
				Damage *= 0;
			}
		}

		if (AttackerContextString == "Player") {
			OpponentTeam[OpponentPokemonId].RecieveDamage(Damage);
			PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
			CameraContext = "Opponent";
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId].RecieveDamage(Damage);
			OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
			CameraContext = "Player";
		}

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + AttackMove->Name.ToString() + " and dealt " + FString::FromInt(Damage) + " damage!";
	}

	
	if (Attacker.Moves[MoveId].MoveStructType == "Status") {
		FStatusMoveStruct* StatusMove = StatusMovesDT->FindRow<FStatusMoveStruct>(Attacker.Moves[MoveId].MoveID, "");

		MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + ", but it failed!";
		bool bIsMoveSuccessful = false;

		for (EEffect Effect : StatusMove->MoveEffects) {
			if (Effect == EEffect::RestoreHP) {
				switch (StatusMove->Target) {
					case ETarget::Self:
						if(Attacker.RestoreHP(Attacker.MaxHP / 2) == true) bIsMoveSuccessful = true;

						break;

					case ETarget::Opponent:
						if(Opponent.RestoreHP(Opponent.MaxHP / 2) == true) bIsMoveSuccessful = true;

						break;
				}
			}

			else {
				switch (StatusMove->Target) {
					case ETarget::Self:
						if (Attacker.AddEffect(Effect) == true) bIsMoveSuccessful = true;
				
						break;

					case ETarget::Opponent:
						if (Opponent.AddEffect(Effect) == true) bIsMoveSuccessful = true;
			
						break;
				}
			}
		}

		if(bIsMoveSuccessful == true) MoveMessage = Attacker.SpeciesData.Name.ToString() + " used " + StatusMove->Name.ToString() + "!";
		
		if (AttackerContextString == "Player") {
			PlayerController->PokemonParty[PlayerPokemonId] = Attacker;
			OpponentTeam[OpponentPokemonId] = Opponent;

			switch (StatusMove->Target) {
				case ETarget::Self:
					CameraContext = "Player";
					break;
				case ETarget::Opponent:
					CameraContext = "Opponent";
					break;
			}

			PlayerController->PokemonParty[PlayerPokemonId].Moves[MoveId].CurrPowerPoints--;
		}

		else if (AttackerContextString == "Opponent") {
			PlayerController->PokemonParty[PlayerPokemonId] = Opponent;
			OpponentTeam[OpponentPokemonId] = Attacker;

			switch (StatusMove->Target) {
			case ETarget::Opponent:
				CameraContext = "Player";
				break;
			case ETarget::Self:
				CameraContext = "Opponent";
				break;
			}

			//OpponentTeam[OpponentPokemonId].Moves[MoveId].CurrPowerPoints--;
		}
	}

	if (AttackerContextString == "Opponent") {
		MoveMessage = "Opponent " + MoveMessage;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Current Action: " + FString::FromInt(CurrentAction)));
	MoveOutcome(MoveMessage, CameraContext);
}

void ABattleGameMode::MoveOutcome(FString MoveMessage, FString CameraContextString)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FTimerHandle CameraSwitchTimer;
	FTimerHandle MoveMessageTimer;
	FTimerHandle PlayerFaintTimer;
	FTimerHandle OpponentFaintTimer;
	FTimerHandle BattleEndTimer;
	FTimerHandle OpponentSwitchTimer;
	FTimerHandle TurnEndTimer;

	FTimerDelegate MoveMessageDelegate;

	if (CurrentAction == 0) {
		Hud->ShowText(MoveMessage);

		if (CameraContextString == "Player") {
			SwitchToPlayerCamera();
		}
		if (CameraContextString == "Opponent") {
			SwitchToOpponentCamera();
		}
	}
	else {
		MoveMessageDelegate.BindUFunction(Hud, FName("ShowText"), MoveMessage);
		GetWorldTimerManager().SetTimer(MoveMessageTimer, MoveMessageDelegate, CurrentAction, false);

		if (CameraContextString == "Player") {
			GetWorldTimerManager().SetTimer(CameraSwitchTimer, this, &ABattleGameMode::SwitchToPlayerCamera, CurrentAction, false);
		}
		if (CameraContextString == "Opponent") {
			GetWorldTimerManager().SetTimer(CameraSwitchTimer, this, &ABattleGameMode::SwitchToOpponentCamera, CurrentAction, false);
		}
	}
	CurrentAction += 1.5;

	if (PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == true) {
		GetWorldTimerManager().SetTimer(PlayerFaintTimer, this, &ABattleGameMode::PlayerFaints, CurrentAction, false);
		CurrentAction += 1.5;
		bHasTurnEnded = true;

		if (PlayerController->bIsPartyDefeated() == true) {
			GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentAction, false);
			bHasBattleEnded = true;
			bIsBattleVictory = false;
			
		}
		else {
			bDoesPlayerHaveToSwitch = true;
		}
	}

	if (OpponentTeam[OpponentPokemonId].bIsFainted == true && PlayerController->bIsPartyDefeated() == false) {
		GetWorldTimerManager().SetTimer(OpponentFaintTimer, this, &ABattleGameMode::OpponentFaints, CurrentAction, false);
		CurrentAction += 1.5;
		bHasTurnEnded = true;

		if (bIsOpponentDefeated() == true) {
			GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentAction, false);
			bHasBattleEnded = true;
			bIsBattleVictory = true;
			
		}
		else {
			OpponentPokemonId = GetCurrentOpponent();

			FTimerDelegate SwitchOpponent;
			SwitchOpponent.BindUFunction(this, FName("PlaceOpponentPokemon"), OpponentPokemonId);

			GetWorldTimerManager().SetTimer(OpponentSwitchTimer, SwitchOpponent, CurrentAction, false);
			CurrentAction += 1.5;
		}
	}
}

void ABattleGameMode::UseItem()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

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
	CurrentAction += 1.5;
}

void ABattleGameMode::UseBall()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FTimerHandle CatchTimer;
	FTimerHandle BattleEndTimer;

	FBallBaseStruct* Ball = BallsDT->FindRow<FBallBaseStruct>(PlayerController->Inventory[SelectedItemID].ItemID, "");

	OpponentPokemonActor->Destroy();

	FRotator Rotation = FRotator(0, 180, 0);
	FVector Position = FVector(-350, 440, 120);
	FActorSpawnParameters SpawnInfo;

	ThrownBallActor = GetWorld()->SpawnActor<ABallActor>(Ball->BallActor, Position, Rotation, SpawnInfo);

	SwitchToOpponentCamera();
	FString BallMessage = "You threw a " + PlayerController->Inventory[SelectedItemID].Name.ToString() + "...";
	Hud->ShowText(BallMessage);
	CurrentAction += 1.5;

	PlayerController->Inventory.RemoveAt(SelectedItemID);

	int CatchRate = OpponentTeam[OpponentPokemonId].SpeciesData.CatchRate * Ball->CatchMultiplier;
	int RandomPercent = FMath::FMath::RandRange(1, 100);

	if (CatchRate > RandomPercent) {
		bHasBattleEnded = true;
		bIsBattleVictory = true;

		GetWorldTimerManager().SetTimer(CatchTimer, this, &ABattleGameMode::CatchSuccess, CurrentAction, false);
		CurrentAction += 1.5;
		GetWorldTimerManager().SetTimer(BattleEndTimer, this, &ABattleGameMode::BattleEnd, CurrentAction, false);

		return;
	}
	else {
		GetWorldTimerManager().SetTimer(CatchTimer, this, &ABattleGameMode::CatchFail, CurrentAction, false);
		CurrentAction += 1.5;

		return;
	}
}

void ABattleGameMode::CatchSuccess()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	PlayerController->ObtainPokemon(OpponentTeam[OpponentPokemonId]);
	Hud->ShowText("You caught " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + "!");
}

void ABattleGameMode::CatchFail()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ThrownBallActor->Destroy();

	FRotator Rotation;
	Rotation.Yaw = 180;
	FActorSpawnParameters SpawnInfo;
	FVector Position = FVector(-350, 440, 115);

	OpponentPokemonActor = GetWorld()->SpawnActor<AStaticOverworldPokemon>(OpponentTeam[OpponentPokemonId].SpeciesData.PokemonActor, Position, Rotation, SpawnInfo);
	Hud->ShowText(OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " got out!");
}

void ABattleGameMode::OpponentFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SwitchToOpponentCamera();

	OpponentPokemonActor->Destroy();

	int Exp = OpponentTeam[OpponentPokemonId].Level * OpponentTeam[OpponentPokemonId].SpeciesData.BaseExp / 7;

	if (PlayerController->PokemonParty[PlayerPokemonId].GainExp(Exp) == true) {
		PlayerController->PokemonParty[PlayerPokemonId].CheckForNewMoves();
	}

	Hud->ShowText("The opponent " + OpponentTeam[OpponentPokemonId].SpeciesData.Name.ToString() + " was defeated, " +
		PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " recieved " + FString::FromInt(Exp) + " Exp!");

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("OpponentFaints"));
}

void ABattleGameMode::PlayerFaints()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SwitchToPlayerCamera();

	PlayerPokemonActor->Destroy();

	Hud->ShowText(PlayerController->PokemonParty[PlayerPokemonId].SpeciesData.Name.ToString() + " was defeated!");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("player faints"));
}

int ABattleGameMode::SelectOpponentMove()
{
	if (OpponentTeam[OpponentPokemonId].bHasRanOutOfPP() == true) {
		return -1;
	}

	int RandId = FMath::RandRange(0, OpponentTeam[OpponentPokemonId].CurrentMoves.Num() - 1);

	if (OpponentTeam[OpponentPokemonId].Moves[OpponentTeam[OpponentPokemonId].CurrentMoves[RandId]].CurrPowerPoints == 0) {
		RandId = SelectOpponentMove();
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, OpponentTeam[OpponentPokemonId].Moves[OpponentTeam[OpponentPokemonId].CurrentMoves[RandId]].Name.ToString());
	return OpponentTeam[OpponentPokemonId].CurrentMoves[RandId];
}

void ABattleGameMode::BattleStart()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	FTimerHandle SendOutOpponentTimer;
	FTimerHandle SendOutPlayerTimer;
	FTimerHandle StartTimer;

	PlayerPokemonId = PlayerController->GetLeadPokemon();
	OpponentPokemonId = GetCurrentOpponent();

	FTimerDelegate SendOutOpponent;
	FTimerDelegate SendOutPlayer;

	SendOutOpponent.BindUFunction(this, FName("PlaceOpponentPokemon"), OpponentPokemonId);
	SendOutPlayer.BindUFunction(this, FName("PlacePlayerPokemon"), PlayerPokemonId);

	GetWorldTimerManager().SetTimer(SendOutOpponentTimer, SendOutOpponent, 0.001, false);
	GetWorldTimerManager().SetTimer(SendOutPlayerTimer, SendOutPlayer, 1.5, false);
	GetWorldTimerManager().SetTimer(StartTimer, Hud, &ABattleHUD::ShowBattleStartWidget, 3, false);
}

void ABattleGameMode::BattleTurn(EAction PlayerAction)
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	CurrentAction = 0;
	bHasTurnEnded = false;

	if (PlayerAction == EAction::UseMove) {
		if (PlayerController->PokemonParty[PlayerPokemonId].GetSpeed() > OpponentTeam[OpponentPokemonId].GetSpeed()) {
			UseMove(SelectedMoveID, "Player");
			UseMove(SelectOpponentMove(), "Opponent");
			EndTurn();
		}

		else {
			UseMove(SelectOpponentMove(), "Opponent");
			UseMove(SelectedMoveID, "Player");
			EndTurn();
		}	
	}

	if (PlayerAction == EAction::SwitchOut) {
		PlayerController->PokemonParty[PlayerPokemonId].ClearEffects();
		PlayerPokemonId = SelectedPokemonID;
		PlacePlayerPokemon(PlayerPokemonId);
		CurrentAction += 1.5;

		if (!bDoesPlayerHaveToSwitch) {
			UseMove(SelectOpponentMove(), "Opponent");
		}
		EndTurn();
		bDoesPlayerHaveToSwitch = false;
	}

	if (PlayerAction == EAction::UseHealingItem) {
		UseItem();
		UseMove(SelectOpponentMove(), "Opponent");
		EndTurn();
	}

	if (PlayerAction == EAction::UsePokeball) {
		UseBall();
		UseMove(SelectOpponentMove(), "Opponent");
		EndTurn();
	}
}

void ABattleGameMode::EndTurn()
{
	FTimerHandle EndTurnTimer;

	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SelectedItemID = 0;
	SelectedMoveID = 0;
	SelectedPokemonID = 0;

	if (bDoesPlayerHaveToSwitch == true && bHasBattleEnded == false) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("time to switch pokemon"));
		GetWorldTimerManager().SetTimer(EndTurnTimer, Hud, &ABattleHUD::ShowPokemon, CurrentAction, false);
	}

	if (bHasBattleEnded == false && PlayerController->PokemonParty[PlayerPokemonId].bIsFainted == false) {
		GetWorldTimerManager().SetTimer(EndTurnTimer, Hud, &ABattleHUD::ShowBattleStartWidget, CurrentAction, false);
	}
}

void ABattleGameMode::BattleEnd()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	FTimerHandle ExitTimer;

	ResetCamera();

	if (bIsBattleVictory == true) {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		if ((PlayerController->Money + Money) > 1000000) {
			Money = 1000000 - PlayerController->Money;
		}

		PlayerController->Money += Money;

		Hud->ShowText("You won! You recieved " + FString::FromInt(Money) + " money!");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("win"));
	}

	else if (bIsBattleVictory == false) {
		int Money = 0;

		for (FPokemonStruct Pokemon : OpponentTeam) {
			Money += Pokemon.Level * 100;
		}

		if ((PlayerController->Money - Money) < 0) {
			Money = 0 + PlayerController->Money;
		}

		PlayerController->Money -= Money;

		Hud->ShowText("You don't have any Pokemon that can fight! You lost " + FString::FromInt(Money) + " money!");
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("loss"));
	}

	GetWorldTimerManager().SetTimer(ExitTimer, this, &ABattleGameMode::ExitBattleMap, CurrentAction, false);
}

void ABattleGameMode::ResetCamera()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerController->SetViewTargetWithBlend(SceneCamera, 0, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::SwitchToPlayerCamera()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerController->SetViewTargetWithBlend(PlayerCamera, 0, EViewTargetBlendFunction::VTBlend_Linear);
}

void ABattleGameMode::SwitchToOpponentCamera()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	PlayerController->SetViewTargetWithBlend(OpponentCamera, 0, EViewTargetBlendFunction::VTBlend_Linear);
}

FString ABattleGameMode::ETypeToString(ETypes Type)
{
	switch (Type) {
		case ETypes::Normal: 
			return "Normal";
		case ETypes::Grass: 
			return "Grass";
		case ETypes::Fire: 
			return "Fire";
		case ETypes::Water: 
			return "Water";
		case ETypes::Earth: 
			return "Earth";
		case ETypes::Air: 
			return "Air";
		case ETypes::Electric: 
			return "Electric";
		case ETypes::Bug: 
			return "Bug";
		case ETypes::Light: 
			return "Light";
		case ETypes::Dark: 
			return "Dark";
		case ETypes::Dragon: 
			return "Dragon";
		default: 
			return " ";
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

	if (PlayerPokemonId == InId || PlayerController->PokemonParty[InId].bIsFainted == true) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("This pokemon can't enter"));
		return;
	}

	SelectedPokemonID = InId;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("switching pokemon"));
	BattleTurn(EAction::SwitchOut);
}

void ABattleGameMode::SelectItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (PlayerController->Inventory[InId].bUsableInBattle == false) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Can't use this item!"));
		return;
	}

	if (PlayerController->Inventory[InId].ItemStructType == "Ball") {
		if (!PlayerController->bCanObtainMorePokemon()) {
			FTimerHandle ShowTurnStartTimer;

			Hud->ShowText("You can't catch more pokemon!");
			GetWorldTimerManager().SetTimer(ShowTurnStartTimer, Hud, &ABattleHUD::ShowBattleStartWidget, 1.5, false);
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Ball"));
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

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Healing Item"));
		SelectedItemID = InId;
		Hud->ShowPokemon();
	}
}

void ABattleGameMode::SelectPokemonToUseItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Hud->BIsMovePopupInViewport()) {
		return;
	}

	SelectedPokemonID = InId;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using item on " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString()));

	if (bHasSelectedItem) {
		BattleTurn(EAction::UseHealingItem);
	}
}

void ABattleGameMode::SelectMoveToUseItem(int InId)
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SelectedMoveID = InId;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Using ether on " + PlayerController->PokemonParty[SelectedPokemonID].SpeciesData.Name.ToString() +
	" to restore " + PlayerController->PokemonParty[SelectedPokemonID].Moves[SelectedMoveID].Name.ToString()));
	BattleTurn(EAction::UseHealingItem);
}

void ABattleGameMode::ShowPokemonInMenu()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	TArray<FPokemonStruct> Party = PlayerController->PokemonParty;

	for (int i = 0; i < Party.Num(); i++) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
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
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
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

	FPokemonStruct Attacker = PlayerController->PokemonParty[PlayerPokemonId];

	for (int i = 0; i < Attacker.CurrentMoves.Num(); i++) {
		ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		UMoveButtonWidget* MoveButton = CreateWidget<UMoveButtonWidget>(UGameplayStatics::GetGameInstance(GetWorld()), Hud->GetMoveButtonWidgetClass());

		MoveButton->InitButton(Attacker.Moves[Attacker.CurrentMoves[i]].Name, Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints,
			Attacker.Moves[Attacker.CurrentMoves[i]].PowerPoints, Attacker.Moves[Attacker.CurrentMoves[i]].MoveType);
		MoveButton->SetMove(Attacker.CurrentMoves[i]);

		if (Attacker.Moves[Attacker.CurrentMoves[i]].CurrPowerPoints > 0) {
			MoveButton->ButtonClicked.AddDynamic(this, &ABattleGameMode::SelectMove);
		}

		MoveDelegate.Broadcast(MoveButton);
	}
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

	return PlayerController->PokemonParty[PlayerPokemonId].bHasRanOutOfPP();
}

bool ABattleGameMode::bHasToSwitchPokemon()
{
	return bDoesPlayerHaveToSwitch;
}

void ABattleGameMode::ExitBattleMap()
{
	ABattleController* PlayerController = Cast<ABattleController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController == nullptr) {
		return;
	}

	for (int i = 0; i < PlayerController->PokemonParty.Num(); i++) {
		PlayerController->PokemonParty[i].ClearEffects();
	}

	UWorldSaveData* SaveData = Cast<UWorldSaveData>(UGameplayStatics::CreateSaveGameObject(UWorldSaveData::StaticClass()));

	SaveData->InventoryData = PlayerController->Inventory;
	SaveData->PokedexData = PlayerController->Pokedex;
	SaveData->MoneyData = PlayerController->Money;
	SaveData->PartyData = PlayerController->PokemonParty;
	SaveData->StorageData = PlayerController->PokemonStorage;

	SaveData->GameMapData = SavedGameMapData;

	UGameplayStatics::SaveGameToSlot(SaveData, "SaveSlot", 0);

	ABattleHUD* Hud = Cast<ABattleHUD>(PlayerController->GetHUD());

	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");

	Hud->PlayerOwner->SetInputMode(FInputModeGameOnly());
	Hud->PlayerOwner->bShowMouseCursor = false;
}

void ABattleGameMode::Run()
{
	ABattleHUD* Hud = Cast<ABattleHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	FTimerHandle ExitTimer;

	Hud->ShowText("Got away safely...");
	GetWorldTimerManager().SetTimer(ExitTimer, this, &ABattleGameMode::ExitBattleMap, 2, false);
}

