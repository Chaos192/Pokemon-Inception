// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "TitleWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnControllsClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnResetClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Play;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Controlls;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Quit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Reset;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked PlayClicked;
	FButtonClicked ControllsClicked;
	FButtonClicked QuitClicked;
	FButtonClicked ResetClicked;
};
