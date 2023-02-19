// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../WildPokemon_AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BB_Keys.h"

UFindPlayerLocation::UFindPlayerLocation()
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector const PlayerLocation = Player->GetActorLocation();
	FNavLocation Location;

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavigationSystem->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Location, nullptr)) {
		Controller->getBlackboard()->SetValueAsVector(bb_keys::targetLocation, Location.Location);
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
