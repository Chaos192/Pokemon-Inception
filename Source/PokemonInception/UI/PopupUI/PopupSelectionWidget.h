// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../ButtonClick.h"
#include "PopupSelectionWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPopupSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	int Id;

	UFUNCTION()
	void OnActionClicked();

	UFUNCTION()
	void OnCancelClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Action;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Cancel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ActionText;

public:
	virtual void NativeOnInitialized() override;

	void SetActionText(FText Text);
	
	void SetId(int InId);

	FElementIDSignature ActionClicked;
	FButtonClicked CancelClicked;
};
