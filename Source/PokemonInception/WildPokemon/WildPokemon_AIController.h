// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "WildPokemon_AIController.generated.h"

UCLASS()
class POKEMONINCEPTION_API AWildPokemon_AIController : public AAIController
{
	GENERATED_BODY()

public:
	AWildPokemon_AIController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	void BeginPlay() override;

	void OnPossess(APawn* const pawn) override;

	class UBlackboardComponent* GetBlackboard();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTreeComponent* BTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BTree;

private:
	class UBlackboardComponent* BlackBoard;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

	void SetupPerception();

	UFUNCTION()
	void PlayerDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnUpdated(TArray<AActor*> const& UpdatedActors);
};
