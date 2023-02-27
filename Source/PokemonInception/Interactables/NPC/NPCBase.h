// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "../InteractableInterface.h"
#include "../../Player/PokemonInceptionCharacter.h"
#include "NPCBase.generated.h"


UCLASS()
class POKEMONINCEPTION_API ANPCBase : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	class UWidgetComponent* OverheadNameWidget;

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	virtual void Interact(APlayerController* PlayerController) override;
};
