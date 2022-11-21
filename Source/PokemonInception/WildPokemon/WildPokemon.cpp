// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../Pokemon/PokemonBase.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"


AWildPokemon::AWildPokemon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AWildPokemon::OnBeginOverlap);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AWildPokemon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWildPokemon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWildPokemon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AWildPokemon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (IsValid(Player)) {
		UGameplayStatics::OpenLevel(GetWorld(), FName("BattleMap"));
	}
}

