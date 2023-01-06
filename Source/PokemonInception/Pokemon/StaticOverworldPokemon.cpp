// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticOverworldPokemon.h"
#include "Components/SkeletalMeshComponent.h"


AStaticOverworldPokemon::AStaticOverworldPokemon()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}



