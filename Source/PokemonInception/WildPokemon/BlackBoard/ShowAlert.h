// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ShowAlert.generated.h"


UCLASS()
class POKEMONINCEPTION_API UShowAlert : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UShowAlert();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
};
