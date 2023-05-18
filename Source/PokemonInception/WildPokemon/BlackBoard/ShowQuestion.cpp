// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowQuestion.h"
#include "../WildPokemon_AIController.h"
#include "../WildPokemon.h"


UShowQuestion::UShowQuestion()
{
	NodeName = TEXT("Show Question");
}

EBTNodeResult::Type UShowQuestion::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	if (!IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}

	AWildPokemon* PossesedPokemon = Cast<AWildPokemon>(Controller->GetPawn());
	if (!IsValid(PossesedPokemon)) {
		return EBTNodeResult::Failed;
	}

	PossesedPokemon->ShowQuestion();
	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
