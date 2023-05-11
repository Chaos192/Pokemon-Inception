// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "../WildPokemon_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BB_Keys.h"

UStopInvestigating::UStopInvestigating()
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	Controller->getBlackboard()->SetValueAsBool(bb_keys::IsInvestigating, false);

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
