// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleController.h"

bool ABattleController::bIsPartyDefeated()
{
	for (FPokemonStruct Pokemon : PokemonParty) {
		if (Pokemon.bIsFainted == false) {
			return false;
		}
	}
	return true;
}

int ABattleController::GetLeadPokemon()
{
	for (int i = 0; i < PokemonParty.Num(); i++) {
		if (PokemonParty[i].bIsFainted == false) {
			return i;
		}
	}

	return -1;
}

void ABattleController::ObtainPokemon(FPokemonStruct Pokemon)
{
	if (bIsRegisteredInPokedex(Pokemon.SpeciesData.PokemonID) == false) {
		RegisterToPokedex(Pokemon.SpeciesData);
	}

	if (PokemonParty.Num() == 6) {
		PokemonStorage.Add(Pokemon);
	}
	else PokemonParty.Add(Pokemon);
}

void ABattleController::RegisterToPokedex(FPokemonBaseStruct Species)
{
	Pokedex.Add(Species);
}

bool ABattleController::bIsRegisteredInPokedex(FName ID)
{
	for (FPokemonBaseStruct PokemonData : Pokedex) {
		if (PokemonData.PokemonID == ID) {
			return true;
		}
	}
	return false;
}
