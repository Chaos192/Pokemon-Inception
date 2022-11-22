// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftPokemon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../UI/OverworldHUD.h"
#include "Kismet/GameplayStatics.h"

AGiftPokemon::AGiftPokemon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AGiftPokemon::OnOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &AGiftPokemon::OnEndOverlap);
}

void AGiftPokemon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	AOverworldHUD* hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Player != nullptr) {
		//Player->InteractDelegate.Unbind();
		//Player->InteractDelegate.BindUFunction(this, FName("GetPokemon"));
		hud->OnScreenMessage("Press 'E' to pick up");
	}
}

void AGiftPokemon::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(OtherActor);
	AOverworldHUD* hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Player != nullptr) {
		//Player->InteractDelegate.Unbind();
		hud->Clear();
	}
}

void AGiftPokemon::GetPokemon()
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AOverworldHUD* hud = Cast<AOverworldHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());

	if (Player != nullptr) {
		//Player->ObtainPokemon(Cast<APokemonBase>(Pokemon));
		hud->ShowText("You recieved a Pokemon");
		Destroy();
	}

}



