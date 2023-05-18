// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Player/PlayerCharacterController.h"
#include "../GameModes/OverworldGameMode.h"
#include "../UI/HUD/OverworldHUD.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NiagaraFunctionLibrary.h"


AWildPokemon::AWildPokemon()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AWildPokemon::Collide);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AWildPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(PlayerController->GetPawn());
	if (!IsValid(Player)) {
		return;
	}

	if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) > 3000) {
		Destroy();
	}
}

void AWildPokemon::InitPokemon(UDataTable* PokemonDatatable, int Level, TArray<UDataTable*> MoveTables)
{
	FPokemonBaseStruct* PokemonSpecies = PokemonDatatable->FindRow<FPokemonBaseStruct>(PokemonID, ""); 

	if (PokemonSpecies) {
		Pokemon.Init(Level, *PokemonSpecies);
		Pokemon.InitMoves(MoveTables);
	}
}

void AWildPokemon::ShowQuestion()
{
	if (Question) {
		FVector SplashLocation = GetActorLocation();
		SplashLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Question, SplashLocation, GetActorRotation());
	}
}

void AWildPokemon::ShowAlert()
{
	if (Alert) {
		FVector SplashLocation = GetActorLocation();
		SplashLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Alert, SplashLocation, GetActorRotation());
	}
}

void AWildPokemon::Collide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode)) {
		return;
	}

	if (GameMode->bIsPeacefulModeOn) {
		return;
	}

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (!IsValid(Player)) {
		return;
	}

	APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PlayerController)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(PlayerController->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	FTimerHandle BattleStartTimer;
	FTimerDelegate BattleStartDelegate;

	TArray<FPokemonStruct> Opponent;
	Opponent.Add(Pokemon);

	GameMode->PauseGame(EPause::Pause);
	GameMode->SaveGame();
	GameMode->SaveLevelData(this);
	GameMode->PlayEncounterSequence();
	Hud->Clear();

	BattleStartDelegate.BindUFunction(GameMode, FName("InitiateBattle"), Opponent, false, true);
	GetWorldTimerManager().SetTimer(BattleStartTimer, BattleStartDelegate, 1.5, false);
}

