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
	void OnPossess(APawn* const pawn)override;
	class UBlackboardComponent* getBlackboard()const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BTree_component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight;

	UFUNCTION()
	void PlayerDetected(AActor* actor, FAIStimulus const stimulus);

	void SetupPerception();
	
};
