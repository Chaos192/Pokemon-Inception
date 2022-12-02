// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/OverworldUI/OverworldHUD.h"


AInteractable::AInteractable()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnEndOverlap);
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractable::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Player) {
		Hud->OnScreenMessage("Press E to " + Action);
	}
}

void AInteractable::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	AOverworldHUD* Hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Player) {
		Hud->ClearOnScreenMessage();
	}
}


