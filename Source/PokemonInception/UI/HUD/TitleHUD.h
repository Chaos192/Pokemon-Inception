// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../TitleUI/ControllsWidget.h"
#include "../TitleUI/TitleWidget.h"
#include "../TitleUI/PlayerNameWidget.h"
#include "../PopupUI/PopupSelectionWidget.h"
#include "TitleHUD.generated.h"


UCLASS()
class POKEMONINCEPTION_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTitleWidget> TitleWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UControlsWidget> ControlsWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerNameWidget> PlayerNameWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UPopupSelectionWidget> ResetProgressPopupClass;

	UPROPERTY()
	class UTitleWidget* TitleWidget;

	UPROPERTY()
	class UControlsWidget* ControlsWidget;

	UPROPERTY()
	class UPlayerNameWidget* PlayerNameWidget;

	UPROPERTY()
	class UPopupSelectionWidget* ResetProgressPopup;

	virtual void BeginPlay()override;

public:
	void Clear();

	UFUNCTION()
	void ShowTitle();

	UFUNCTION()
	void ShowControlls();

	UFUNCTION()
	void ShowPlayerNameInput();

	UFUNCTION()
	void ShowResetProgressPopup();
};
