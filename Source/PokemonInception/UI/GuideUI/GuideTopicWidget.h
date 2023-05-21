// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "GuideTopicWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UGuideTopicWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Back;

public:
	virtual void NativeOnInitialized() override;

	FButtonClicked BackClicked;
};
