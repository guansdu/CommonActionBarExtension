// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EnhancedActionBarBPLibrary.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class COMMONACTIONBAREXTENSION_API UEnhancedActionBarBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/***
	 * Register an action to the action bar.
	 */
	UFUNCTION(BlueprintCallable, Category = "EnhancedActionBar")
	static void RegisterEnhancedAction(const UUserWidget* InWidget, const UInputAction* InAction);
};
