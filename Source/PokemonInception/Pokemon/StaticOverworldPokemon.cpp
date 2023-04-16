// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticOverworldPokemon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

void AStaticOverworldPokemon::PlayCry()
{
	if (Cry != nullptr) {
		UGameplayStatics::PlaySound2D(GetWorld(), Cry);
	}
}

void AStaticOverworldPokemon::Roar()
{
	if (RoarAnimMontage != nullptr) { 
		PlayAnimMontage(RoarAnimMontage, 1);
	}

	FTimerHandle CryTimer;
	GetWorldTimerManager().SetTimer(CryTimer, this, &AStaticOverworldPokemon::PlayCry, 0.4, false);
}

void AStaticOverworldPokemon::Attack()
{
	if (AttackAnimMontage != nullptr) {
		PlayAnimMontage(AttackAnimMontage, 1);
	}
}

void AStaticOverworldPokemon::GetDamaged()
{
	if (DamageAnimMontage != nullptr) {
		PlayAnimMontage(DamageAnimMontage, 1);
	}
}

void AStaticOverworldPokemon::Hide(bool bIsHidden)
{
	if (bIsHidden) {
		SetActorScale3D(FVector(0.001, 0.001, 0.001));
	}
	else {
		SetActorScale3D(FVector(1, 1, 1));
	}
}

void AStaticOverworldPokemon::ComeOut(float Scale)
{
	if (Scale <= 0.05f && BallOpenSound) {
		ShowBallOpen();
		UGameplayStatics::PlaySound2D(GetWorld(), BallOpenSound, 1);
	}

	if (Scale >= 1.f) {
		Roar();
		return;
	}
	
	SetActorScale3D(FVector(Scale, Scale, Scale));

	ChangeSizeDelegate.BindUFunction(this, FName("ComeOut"), Scale += 0.05f);
	GetWorldTimerManager().SetTimer(ChangeSizeTimer, ChangeSizeDelegate, 0.00001, false);
}

void AStaticOverworldPokemon::ShowBallOpen()
{
	if (BallOpen != nullptr) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BallOpen, GetActorLocation());
	}
}

void AStaticOverworldPokemon::CastMove(FMoveBaseStruct Move)
{
	if (Move.MoveStructType == "Attack") {
		Attack();
	}
	else {
		Roar();
	}
}

void AStaticOverworldPokemon::GetSplashed(FMoveBaseStruct Move)
{
	if (Move.Particle) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Move.Particle, GetActorLocation());
	}

	if (Move.Sound) {
		UGameplayStatics::PlaySound2D(GetWorld(), Move.Sound);
	}

	if (Move.MoveStructType == "Attack") {
		GetDamaged();
	}
}

void AStaticOverworldPokemon::Faint(float Scale, bool bIsTrainerPokemon)
{
	if (Scale == 1) {
		GetDamaged();
	}

	if (Scale <= 0.02 && Cry != nullptr) {
		if (bIsTrainerPokemon && BallRetrieveSound) {
			ShowBallOpen();
			UGameplayStatics::PlaySound2D(GetWorld(), BallRetrieveSound, 1);
		}

		UGameplayStatics::PlaySound2D(GetWorld(), Cry, 1, 0.75);
		Destroy();
		return;
	}
	
	SetActorScale3D(FVector(Scale, Scale, Scale));

	ChangeSizeDelegate.BindUFunction(this, FName("Faint"), Scale -= 0.05, bIsTrainerPokemon);
	GetWorldTimerManager().SetTimer(ChangeSizeTimer, ChangeSizeDelegate, 0.00001, false);
}
