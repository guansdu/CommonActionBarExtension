// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"

#include "EnhancedActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class COMMONACTIONBAREXTENSION_API UEnhancedActionWidget : public UCommonActionWidget
{
	GENERATED_BODY()

public:
	virtual FSlateBrush GetIcon() const override;
};
