// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "WildPokemon_AIController.h"
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
	AWildPokemon_AIController* cont = Cast<AWildPokemon_AIController>(ownerComp.GetAIOwner());
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector const PlayerLocation = player->GetActorLocation();
	FNavLocation location;

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (navSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, location, nullptr)) {
		cont->getBlackboard()->SetValueAsVector(bb_keys::targetLocation, location.Location);
	}

	FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
