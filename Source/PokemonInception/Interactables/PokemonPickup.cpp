// Fill out your copyright notice in the Description page of Project Settings.


#include "PokemonPickup.h"

APokemonPickup::APokemonPickup()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Action = "pick up";
	//Pokemon->Init(PokemonLevel);
}

void APokemonPickup::Interact(APlayerController* Controller)
{
	//h
}
