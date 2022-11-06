// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "PokemonInceptionCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BB_Keys.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


AWildPokemon_AIController::AWildPokemon_AIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("BehaviorTree'/Game/AI/WildPokemon/BT_WildPokemon.BT_WildPokemon'"));
	if (obj.Succeeded()) {
		BTree = obj.Object;
	}
	BTree_component = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BTComp"));
	blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	SetupPerception();
}

void AWildPokemon_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTree);
	BTree_component->StartTree(*BTree);
}

void AWildPokemon_AIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard) {
		blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AWildPokemon_AIController::getBlackboard() const
{
	return blackboard;
}

void AWildPokemon_AIController::PlayerDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (APokemonInceptionCharacter* const character = Cast<APokemonInceptionCharacter>(actor)) {
		getBlackboard()->SetValueAsBool(bb_keys::CanSeePlayer, stimulus.WasSuccessfullySensed());
	}
}

void AWildPokemon_AIController::SetupPerception()
{
	sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	sight->SightRadius = 500.0f;
	sight->LoseSightRadius = sight->SightRadius + 50.0f;
	sight->PeripheralVisionAngleDegrees = 135.0f;
	sight->SetMaxAge(5.0f);
	sight->AutoSuccessRangeFromLastSeenLocation = 10.0f;
	sight->DetectionByAffiliation.bDetectEnemies = true;
	sight->DetectionByAffiliation.bDetectFriendlies = true;
	sight->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*sight->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWildPokemon_AIController::PlayerDetected);
	GetPerceptionComponent()->ConfigureSense(*sight);
}
