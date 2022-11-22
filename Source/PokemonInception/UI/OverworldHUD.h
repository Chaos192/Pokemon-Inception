// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuWidget.h"
#include "TextBoxWidget.h"
#include "OverworldHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API AOverworldHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> TextBoxWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> OnScreenMessageWidgetClass;

	UPROPERTY()
	class UMenuWidget* MenuWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	UPROPERTY()
	class UTextBoxWidget* OnScreenMessageWidget;

	virtual void BeginPlay()override;
	
public:
	void Clear();

	UFUNCTION()
	void ShowMenu();

	UFUNCTION()
	void OnScreenMessage(FString Message);

	UFUNCTION()
	void ShowText(FString Message);

	UFUNCTION()
	void TogglePause(bool IsPaused);

};
