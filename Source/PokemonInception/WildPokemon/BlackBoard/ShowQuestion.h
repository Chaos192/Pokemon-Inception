// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ShowQuestion.generated.h"


UCLASS()
class POKEMONINCEPTION_API UShowQuestion : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UShowQuestion();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
};
