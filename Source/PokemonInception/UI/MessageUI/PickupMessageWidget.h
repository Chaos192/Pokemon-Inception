// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextBoxWidget.h"
#include "Components/Image.h"
#include "PickupMessageWidget.generated.h"


UCLASS()
class POKEMONINCEPTION_API UPickupMessageWidget : public UTextBoxWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UImage* Image;

public:
	UFUNCTION()
	void SetImage(UTexture2D* InImage);
};
