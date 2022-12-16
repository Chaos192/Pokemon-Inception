// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ButtonClick.h"
#include "ItemInfoWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UItemInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnUseClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Use;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DescriptionText;

	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetDescription(FText Description);

	FButtonClicked UseClicked;
};
