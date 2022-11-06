// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "WildPokemon_AIController.h"
#include "BB_Keys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
//#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
//#include "GameFramework/Character.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UChasePlayer::UChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
	AWildPokemon_AIController* cont = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	FVector const PlayerLocation = cont->getBlackboard()->GetValueAsVector(bb_keys::targetLocation);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, PlayerLocation);

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
