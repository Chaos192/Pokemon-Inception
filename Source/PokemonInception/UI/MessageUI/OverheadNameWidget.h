// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "OverheadNameWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UOverheadNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Name;

public:
	UFUNCTION()
	void SetName(FString InName);
};
