// Fill out your copyright notice in the Description page of Project Settings.


#include "GiftItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AGiftItem::AGiftItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(Mesh);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AGiftItem::GetItem);
}

void AGiftItem::GetItem(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (Player != nullptr) {
		//Player->ObtainItem(Cast<FItemBaseStruct>(Item));
		//hud->ShowText("You recieved a Pokemon");
		Destroy();
	}
}


