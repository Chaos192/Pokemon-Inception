// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"

void USettingsWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USettingsWidget::OnSaveSettingsClicked()
{
	SaveSettingsClicked.Broadcast();
}

void USettingsWidget::OnGraphicsQualityChanged(float InValue)
{
	GraphicsQualityChanged.Broadcast(InValue);
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "Graphics Quality: " + FString::SanitizeFloat(InValue));
}

void USettingsWidget::OnSEVolumeChanged(float InValue)
{
	SEVolumeChanged.Broadcast(InValue);
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "SE Volume: " + FString::SanitizeFloat(InValue));
}

void USettingsWidget::OnBGMVolumeChanged(float InValue)
{
	BGMVolumeChanged.Broadcast(InValue);
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Yellow, "BGM Volume: " + FString::SanitizeFloat(InValue));
}

void USettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &USettingsWidget::OnBackClicked);
	SaveSettings->OnClicked.AddDynamic(this, &USettingsWidget::OnSaveSettingsClicked);

	GraphicsQualitySlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnGraphicsQualityChanged);
	SEVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnSEVolumeChanged);
	BGMVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnBGMVolumeChanged);
}
