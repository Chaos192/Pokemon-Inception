// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"
#include "../WildPokemon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "../WildPokemon_AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BB_Keys.h"

UFindRandomLocation::UFindRandomLocation()
{
	NodeName = TEXT("Find a random location");
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}

	AWildPokemon* PossesedPokemon = Cast<AWildPokemon>(Controller->GetPawn());
	if (!IsValid(PossesedPokemon)) {
		return EBTNodeResult::Failed;
	}

	PossesedPokemon->GetCharacterMovement()->MaxWalkSpeed = PossesedPokemon->NormalSpeed;

	FVector OriginLocation = PossesedPokemon->GetActorLocation();
	FNavLocation NewLocation;

	UNavigationSystemV1* const NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavigationSystem->GetRandomPointInNavigableRadius(OriginLocation, SearchRadius, NewLocation, nullptr)) {
		Controller->GetBlackboard()->SetValueAsVector(bb_keys::targetLocation, NewLocation.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
