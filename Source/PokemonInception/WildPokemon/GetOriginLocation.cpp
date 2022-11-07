// Fill out your copyright notice in the Description page of Project Settings.


#include "GetOriginLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "WildPokemon_AIController.h"
#include "WildPokemon.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BB_Keys.h"

UGetOriginLocation::UGetOriginLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Get the Origin Point");
}

EBTNodeResult::Type UGetOriginLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	auto const cont = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	auto const npc = cont->GetPawn();

	AWildPokemon* const WildPokemon = Cast<AWildPokemon>(npc);
	
	if (IsValid(WildPokemon)) {
		cont->getBlackboard()->SetValueAsVector(bb_keys::originPoint, WildPokemon->GetOriginPoint());
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
