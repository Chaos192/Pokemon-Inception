// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "../ButtonClick.h"
#include "PopupSelectionWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPopupSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	int Id;

	UFUNCTION()
	void OnActionClicked();

	UFUNCTION()
	void OnCancelClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Action;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Cancel;

	virtual void NativeConstruct() override;

public:
	FElementIDSignature ActionClicked;
	FButtonClicked CancelClicked;

	void SetId(int InId);
};
