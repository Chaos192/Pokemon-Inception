// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowAlert.h"
#include "../WildPokemon_AIController.h"
#include "../WildPokemon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BB_Keys.h"


UShowAlert::UShowAlert()
{
	NodeName = TEXT("Show Alert");
}

EBTNodeResult::Type UShowAlert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}

	AWildPokemon* PossesedPokemon = Cast<AWildPokemon>(Controller->GetPawn());
	if (!IsValid(PossesedPokemon)) {
		return EBTNodeResult::Failed;
	}

	PossesedPokemon->ShowAlert();
	Controller->GetBlackboard()->SetValueAsBool(bb_keys::PlayerJustSpotted, false);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
