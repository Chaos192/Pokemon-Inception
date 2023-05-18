// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pokemon/PokemonBaseStruct.h"
#include "../Pokemon/PokemonStruct.h"
#include "../Pokemon/StaticOverworldPokemon.h"
#include "WildPokemon.generated.h"


UCLASS()
class POKEMONINCEPTION_API AWildPokemon : public AStaticOverworldPokemon
{
	GENERATED_BODY()

public:
	AWildPokemon();

	UPROPERTY()
	FPokemonStruct Pokemon;

	UPROPERTY(EditDefaultsOnly)
	int NormalSpeed = 100;

	UPROPERTY(EditDefaultsOnly)
	int RunningSpeed = 200;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitPokemon(UDataTable* PokemonDatatable, int Level, TArray<UDataTable*> MoveTables);

	void ShowQuestion();
	void ShowAlert();

private:
	UPROPERTY(EditDefaultsOnly)
	FName PokemonID;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* Question = nullptr;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraSystem* Alert = nullptr;

	UFUNCTION()
	void Collide(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
