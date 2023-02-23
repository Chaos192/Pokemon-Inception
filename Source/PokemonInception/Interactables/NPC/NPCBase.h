// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "../InteractableInterface.h"
#include "../../UI/MessageUI/OverheadNameWidget.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UOverheadNameWidget> OverheadNameWidgetClass;

	virtual void Interact(APlayerController* PlayerController) override;
};
