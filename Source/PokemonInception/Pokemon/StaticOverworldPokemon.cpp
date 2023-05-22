// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticOverworldPokemon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


void AStaticOverworldPokemon::Roar()
{
	if (RoarAnimMontage) { 
		PlayAnimMontage(RoarAnimMontage, 1);
	}
}

void AStaticOverworldPokemon::Attack()
{
	if (AttackAnimMontage) {
		PlayAnimMontage(AttackAnimMontage, 1);
	}
}

void AStaticOverworldPokemon::GetDamaged()
{
	if (DamageAnimMontage) {
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
	if (BallOpen) {
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
		FVector SplashLocation = GetActorLocation();
		SplashLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Move.Particle, SplashLocation);
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
	if (Scale == 1 && FaintAnimMontage) {
		PlayAnimMontage(FaintAnimMontage, 1);
	}

	if (Scale <= 0.02) {
		if (bIsTrainerPokemon && BallRetrieveSound) {
			ShowBallOpen();
			UGameplayStatics::PlaySound2D(GetWorld(), BallRetrieveSound, 1);
		}

		Destroy();
		return;
	}
	
	SetActorScale3D(FVector(Scale, Scale, Scale));

	ChangeSizeDelegate.BindUFunction(this, FName("Faint"), Scale -= 0.05, bIsTrainerPokemon);
	GetWorldTimerManager().SetTimer(ChangeSizeTimer, ChangeSizeDelegate, 0.001, false);
}
