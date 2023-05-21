// Fill out your copyright notice in the Description page of Project Settings.


#include "WildPokemon_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h" 
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "WildPokemon.h"
#include "../Player/PokemonInceptionCharacter.h"
#include "../GameModes/OverworldGameMode.h"
#include "BlackBoard/BB_Keys.h"
#include "BlackBoard/AI_Tags.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


AWildPokemon_AIController::AWildPokemon_AIController(FObjectInitializer const& ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("BehaviorTree'/Game/Blueprints/Pokemon/BehaviorTree/BT_WildPokemon.BT_WildPokemon'"));
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

UBlackboardComponent* AWildPokemon_AIController::GetBlackboard()
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

	APokemonInceptionCharacter* Player = Cast<APokemonInceptionCharacter>(actor);
	if (!IsValid(Player)) {
		return;
	}

	if (!BlackBoard->GetValueAsBool(bb_keys::CanSeePlayer)) {
		BlackBoard->SetValueAsBool(bb_keys::PlayerJustSpotted, stimulus.WasSuccessfullySensed());
	}

	BlackBoard->SetValueAsBool(bb_keys::CanSeePlayer, stimulus.WasSuccessfullySensed());
}

void AWildPokemon_AIController::OnUpdated(TArray<AActor*> const& UpdatedActors)
{
	AOverworldGameMode* GameMode = Cast<AOverworldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(GameMode)) {
		return;
	}

	if (GameMode->bIsPeacefulModeOn) {
		return;
	}

	for (AActor* Actor : UpdatedActors) {
		FActorPerceptionBlueprintInfo Info;
		GetPerceptionComponent()->GetActorsPerception(Actor, Info);

		for (int i = 0; i < Info.LastSensedStimuli.Num(); i++) {
			FAIStimulus const Stimuli = Info.LastSensedStimuli[i];

			if (Stimuli.Tag == AI_Tags::NoiseTag) {
				BlackBoard->SetValueAsBool(bb_keys::IsInvestigating, Stimuli.WasSuccessfullySensed());
				BlackBoard->SetValueAsVector(bb_keys::targetLocation, Stimuli.StimulusLocation);
			}
			else {
				BlackBoard->SetValueAsBool(bb_keys::CanSeePlayer, Stimuli.WasSuccessfullySensed());
			}
		}
	}
}

void AWildPokemon_AIController::SetupPerception()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (Sight) {
		Sight->SightRadius = 1000.0f;
		Sight->LoseSightRadius = Sight->SightRadius + 50.0f;
		Sight->PeripheralVisionAngleDegrees = 135.0f;
		Sight->SetMaxAge(5.0f);
		Sight->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = true;
		Sight->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWildPokemon_AIController::PlayerDetected);
		GetPerceptionComponent()->ConfigureSense(*Sight);
	}
	
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (Hearing) {
		Hearing->HearingRange = 1500;
		Hearing->DetectionByAffiliation.bDetectEnemies = true;
		Hearing->DetectionByAffiliation.bDetectFriendlies = true;
		Hearing->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AWildPokemon_AIController::OnUpdated);
		GetPerceptionComponent()->ConfigureSense(*Hearing);
	}
}
