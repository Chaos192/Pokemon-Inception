// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../GameModes/OverworldGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BlackBoard/BB_Keys.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


AWildPokemon_AIController::AWildPokemon_AIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("BehaviorTree'/Game/AI/WildPokemon/BehaviourTree/BT_WildPokemon.BT_WildPokemon'"));
	if (obj.Succeeded()) {
		BTree = obj.Object;
	}

	BTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BTComp"));
	BlackBoard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	SetupPerception();
}

void AWildPokemon_AIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BTree);
	BTreeComponent->StartTree(*BTree);
}

void AWildPokemon_AIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (Blackboard) {
		Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent* AWildPokemon_AIController::getBlackboard() const
{
	return BlackBoard;
}

void AWildPokemon_AIController::PlayerDetected(AActor* actor, FAIStimulus const stimulus)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (GameMode->bIsPeacefulModeOn) {
		return;
	}

	if (APokemonInceptionCharacter* const Player = Cast<APokemonInceptionCharacter>(actor)) {
		getBlackboard()->SetValueAsBool(bb_keys::CanSeePlayer, stimulus.WasSuccessfullySensed());
	}
}

void AWildPokemon_AIController::SetupPerception()
{
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	Sight->SightRadius = 500.0f;
	Sight->LoseSightRadius = Sight->SightRadius + 50.0f;
	Sight->PeripheralVisionAngleDegrees = 135.0f;
	Sight->SetMaxAge(5.0f);
	Sight->AutoSuccessRangeFromLastSeenLocation = 10.0f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWildPokemon_AIController::PlayerDetected);
	GetPerceptionComponent()->ConfigureSense(*Sight);
}
