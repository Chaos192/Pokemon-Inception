// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "BattleHUD.h"

void ABattleGameMode::Run()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TestingMap");
}

