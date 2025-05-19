// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionBar.h"
#include "EnhancedBoundActionBar.generated.h"

/**
 * 
 */
UCLASS()
class COMMONACTIONBAREXTENSION_API UEnhancedBoundActionBar : public UCommonBoundActionBar
{
	GENERATED_BODY()

protected:
	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	virtual void ActionBarUpdateBeginImpl() override;
	virtual void ActionBarUpdateEndImpl() override;

	virtual void NativeOnEnhancedActionsUpdated();

private:
	void HandleOnActionsUpdated();

	bool bIsPendingRefresh = false;
};
