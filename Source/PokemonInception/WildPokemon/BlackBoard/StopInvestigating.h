// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StopInvestigating.generated.h"


UCLASS()
class POKEMONINCEPTION_API UStopInvestigating : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UStopInvestigating();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);

};
