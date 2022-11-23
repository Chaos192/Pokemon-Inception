// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "PokemonWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPokemonWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnBackClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Back;

	virtual void NativeConstruct() override;

public:
	FButtonClicked BackClicked;
};
