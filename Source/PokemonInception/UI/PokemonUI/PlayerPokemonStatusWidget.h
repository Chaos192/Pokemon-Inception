// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PokemonStatusWidget.h"
#include "PlayerPokemonStatusWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPlayerPokemonStatusWidget : public UPokemonStatusWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* EXPBar;

public:
	UFUNCTION()
	void SetPokemonEXP(int CurrEXP, int MaxEXP);
};
