// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionButton.h"
#include "EnhancedBoundActionButton.generated.h"

class UEnhancedActionWidget;
class UInputAction;

/**
 * 
 */
UCLASS()
class COMMONACTIONBAREXTENSION_API UEnhancedBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()

public:
	virtual void SetRepresentedAction(FUIActionBindingHandle InBindingHandle) override;

	virtual void SetRepresentEnhancedAction(const UInputAction* InAction);

protected:
	virtual void UpdateInputActionWidget() override;

	UPROPERTY(BlueprintReadOnly, Category = Input, meta = (BindWidget, OptionalWidget = true, AllowPrivateAccess = true))
	TObjectPtr<UCommonActionWidget> ChordedInputActionWidget;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UInputAction> AssociatedInputAction;
};
