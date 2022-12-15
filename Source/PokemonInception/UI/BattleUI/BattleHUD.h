// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleStartWidget.h"
#include "FightWidget.h"
#include "MoveButtonWidget.h"
#include "../TextBoxWidget.h"
#include "BattleHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API ABattleHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBattleStartWidget> BattleStartWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFightWidget> FightWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMoveButtonWidget> MoveButtonWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTextBoxWidget> TextBoxWidgetClass;

	UPROPERTY()
	class UBattleStartWidget* BattleStartWidget;

	UPROPERTY()
	class UFightWidget* FightWidget;

	UPROPERTY()
	class UTextBoxWidget* TextBoxWidget;

	virtual void BeginPlay() override;
	
public:
	void Clear();

	UFUNCTION()
	void ShowFightWidget();

	UFUNCTION()
	void ShowBattleStartWidget();

	UFUNCTION()
	void ShowWidget(class UUserWidget* Widget);

	UFUNCTION()
	void ShowText(FString Message, class UUserWidget* NextWidget);

};
