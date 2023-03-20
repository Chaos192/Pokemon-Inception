// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaBox.h"
#include "Components/BoxComponent.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../UI/HUD/OverworldHUD.h"
#include "Kismet/GameplayStatics.h"

AAreaBox::AAreaBox()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AAreaBox::OnAreaEntered);
	AreaName = "Area Zero";
}

void AAreaBox::OnAreaEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	if (!IsValid(Player)) {
		return;
	}

	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	if (!IsValid(Hud)) {
		return;
	}

	Hud->ShowAreaName(AreaName);
}
