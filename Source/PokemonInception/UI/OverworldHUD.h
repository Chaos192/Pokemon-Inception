// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuWidget.h"
#include "OverworldHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API AOverworldHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	UPROPERTY()
	class UMenuWidget* MenuWidget;
	
	void Clear();

	virtual void BeginPlay()override;
	
public:

	UFUNCTION()
	void ShowMenu();

	UFUNCTION()
	void TogglePause(bool IsPaused);

};
