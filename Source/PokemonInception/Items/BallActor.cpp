// Fill out your copyright notice in the Description page of Project Settings.


#include "BallActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

ABallActor::ABallActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

void ABallActor::CatchSuccess()
{
	if (BallSparkle) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BallSparkle, GetActorLocation());
	}

	if (BallClick) {
		UGameplayStatics::PlaySound2D(GetWorld(), BallClick);
	}
}

void ABallActor::CatchFail()
{
	if (BallFail) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BallFail, GetActorLocation());
	}

	if (BallBreak) {
		UGameplayStatics::PlaySound2D(GetWorld(), BallBreak);
	}

	Destroy();
}


