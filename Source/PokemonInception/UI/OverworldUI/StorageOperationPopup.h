// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUI/PopupSelectionWidget.h"
#include "Components/TextBlock.h"
#include "StorageOperationPopup.generated.h"


UCLASS()
class POKEMONINCEPTION_API UStorageOperationPopup : public UPopupSelectionWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void OnOperationClicked();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Operation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* OperationText;

public:
	virtual void NativeOnInitialized() override;

	void SetOperationText(FText Text);

	FElementIDSignature OperationClicked;
};
