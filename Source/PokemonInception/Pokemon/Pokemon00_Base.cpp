#include "Pokemon00_Base.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "Pokemon00_Base.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APokemon00_Base::APokemon00_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	SetRootComponent(Capsule);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Mesh->SetupAttachment(Capsule);

	CurrHp = MaxHp;
}

// Called when the game starts or when spawned
void APokemon00_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APokemon00_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

USkeletalMeshComponent* APokemon00_Base::GetPokemonMesh()
{
	return Mesh;
}

