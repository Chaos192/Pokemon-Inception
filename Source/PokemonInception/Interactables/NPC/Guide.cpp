// Fill out your copyright notice in the Description page of Project Settings.


#include "Guide.h"

AGuide::AGuide()
{
	Name = "Guide";
}

void AGuide::Interact(APlayerController* PlayerController)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Emerald, "It's guiding time!");
}
