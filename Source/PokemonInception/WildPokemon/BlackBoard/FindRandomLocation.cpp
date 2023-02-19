// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../WildPokemon_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BB_Keys.h"

UFindRandomLocation::UFindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find a random location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	auto const Controller = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	auto const NPC = Controller->GetPawn();

	FVector const Origin = NPC->GetActorLocation();
	FNavLocation Location;

	UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavigationSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location, nullptr)) {
		Controller->getBlackboard()->SetValueAsVector(bb_keys::targetLocation, Location.Location);
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
