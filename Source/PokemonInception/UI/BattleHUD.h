// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleStartWidget.h"
#include "BattleHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API ABattleHUD : public AHUD
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBattleStartWidget> BattleStartWidgetClass;

	UPROPERTY()
	class UBattleStartWidget* BattleStartWidget;
	
	void Clear();

	virtual void BeginPlay()override;
	
public:

	UFUNCTION()
	void ShowOptions();

};
