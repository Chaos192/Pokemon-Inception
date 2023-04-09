// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticOverworldPokemon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

void AStaticOverworldPokemon::PlayCry()
{
	if (Cry != nullptr) {
		UGameplayStatics::PlaySound2D(GetWorld(), Cry);
	}
}

void AStaticOverworldPokemon::Roar()
{
	if (RoarAnimMontage != nullptr) { 
		PlayAnimMontage(RoarAnimMontage, 1);
	}

	FTimerHandle CryTimer;
	GetWorldTimerManager().SetTimer(CryTimer, this, &AStaticOverworldPokemon::PlayCry, 0.4, false);
}
