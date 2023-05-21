// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../WildPokemon_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BB_Keys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


UChasePlayer::UChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AWildPokemon_AIController* Controller = Cast<AWildPokemon_AIController>(OwnerComp.GetAIOwner());
	if (!IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}

	AWildPokemon* PossesedPokemon = Cast<AWildPokemon>(Controller->GetPawn());
	if (!IsValid(PossesedPokemon)) {
		return EBTNodeResult::Failed;
	}

	FVector PlayerLocation = Controller->GetBlackboard()->GetValueAsVector(bb_keys::targetLocation);
	PossesedPokemon->GetCharacterMovement()->MaxWalkSpeed = PossesedPokemon->RunningSpeed;

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerLocation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
