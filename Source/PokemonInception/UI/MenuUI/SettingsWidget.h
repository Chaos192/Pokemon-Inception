// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "../ButtonClick.h"
#include "SettingsWidget.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValueUpdatedSignature, float, Value);

UCLASS()
class POKEMONINCEPTION_API USettingsWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnGraphicsQualityChanged(float InValue);

	UFUNCTION()
	void OnSEVolumeChanged(float InValue);

	UFUNCTION()
	void OnPokemonCryVolumeChanged(float InValue);

	UFUNCTION()
	void OnBGMVolumeChanged(float InValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USlider* GraphicsQualitySlider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USlider* SEVolumeSlider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USlider* PokemonCryVolumeSlider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	USlider* BGMVolumeSlider;

	virtual void NativeConstruct() override;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked BackClicked;

	FValueUpdatedSignature GraphicsQualityChanged;
	FValueUpdatedSignature SEVolumeChanged;
	FValueUpdatedSignature PokemonCryVolumeChanged;
	FValueUpdatedSignature BGMVolumeChanged;
};
