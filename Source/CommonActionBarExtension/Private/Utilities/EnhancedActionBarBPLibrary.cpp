// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnhancedActionBarBPLibrary.h"

#include "Utilities/EnhancedActionBarSubsystem.h"
#include "Blueprint/UserWidget.h"

void UEnhancedActionBarBPLibrary::RegisterEnhancedAction(const UUserWidget* InWidget, const UInputAction* InAction)
{
	if (InWidget && InAction)
	{
		if (UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(InWidget->GetOwningLocalPlayer()))
		{
			EnhancedActionBarSubsystem->RegisterAction(InWidget, InAction);
		}
	}
}

void UEnhancedActionBarBPLibrary::UnregisterEnhancedAction(const UUserWidget* InWidget, const UInputAction* InAction)
{
	if (InWidget && InAction)
	{
		UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(InWidget->GetOwningLocalPlayer());
		EnhancedActionBarSubsystem->UnregisterAction(InWidget, InAction);
	}
}
