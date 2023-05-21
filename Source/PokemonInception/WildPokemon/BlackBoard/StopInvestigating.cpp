// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigating.h"
#include "../WildPokemon_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BB_Keys.h"

UStopInvestigating::UStopInvestigating()
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UStopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}

	Controller->GetBlackboard()->SetValueAsBool(bb_keys::IsInvestigating, false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
