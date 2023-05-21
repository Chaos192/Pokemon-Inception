// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "TextBoxWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UTextBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Message;

public:
	UFUNCTION()
	void ReturnMessage(FString MessageToDisplay);
};
