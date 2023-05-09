// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../../Settings/PokemonInceptionGameInstance.h"

void USettingsWidget::OnBackClicked()
{
	BackClicked.Broadcast();
}

void USettingsWidget::OnGraphicsQualityChanged(float InValue)
{
	GraphicsQualityChanged.Broadcast(InValue);
}

void USettingsWidget::OnSEVolumeChanged(float InValue)
{
	SEVolumeChanged.Broadcast(InValue);
}

void USettingsWidget::OnPokemonCryVolumeChanged(float InValue)
{
	PokemonCryVolumeChanged.Broadcast(InValue);
}

void USettingsWidget::OnBGMVolumeChanged(float InValue)
{
	BGMVolumeChanged.Broadcast(InValue);
}

void USettingsWidget::NativeConstruct()
{
	UPokemonInceptionGameInstance* GameInstance = Cast<UPokemonInceptionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!IsValid(GameInstance)) {
		return;
	}

	GraphicsQualitySlider->SetValue(GameInstance->GetGraphicsQuality());
	SEVolumeSlider->SetValue(GameInstance->GetSEVolume());
	PokemonCryVolumeSlider->SetValue(GameInstance->GetPokemonCryVolume());
	BGMVolumeSlider->SetValue(GameInstance->GetBGMVolume());
}

void USettingsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	Back->OnClicked.AddDynamic(this, &USettingsWidget::OnBackClicked);

	GraphicsQualitySlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnGraphicsQualityChanged);
	SEVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnSEVolumeChanged);
	PokemonCryVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnPokemonCryVolumeChanged);
	BGMVolumeSlider->OnValueChanged.AddDynamic(this, &USettingsWidget::OnBGMVolumeChanged);
}
