// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnhancedBoundActionButton.h"

#include "CommonActionWidget.h"
#include "CommonTextBlock.h"
#include "Utilities/EnhancedActionBarSubsystem.h"

void UEnhancedBoundActionButton::SetRepresentedAction(FUIActionBindingHandle InBindingHandle)
{
	AssociatedInputAction = nullptr;

	if (InputActionWidget)
	{
		InputActionWidget->SetEnhancedInputAction(nullptr);
	}

	if (ChordedInputActionWidget)
	{
		ChordedInputActionWidget->SetEnhancedInputAction(nullptr);
	}

	Super::SetRepresentedAction(InBindingHandle);
}

void UEnhancedBoundActionButton::SetRepresentEnhancedAction(const UInputAction* InAction)
{
	AssociatedInputAction = InAction;

	if (InputActionWidget)
	{
		InputActionWidget->SetEnhancedInputAction(const_cast<UInputAction*>(InAction));
	}

	UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(GetOwningLocalPlayer());
	if (ChordedInputActionWidget)
	{
		if (const UInputAction* ChordedInputAction = EnhancedActionBarSubsystem->TryFindFirstChordedAction(InAction))
		{
			ChordedInputActionWidget->SetEnhancedInputAction(const_cast<UInputAction*>(ChordedInputAction));
			ChordedInputActionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ChordedInputActionWidget->SetEnhancedInputAction(nullptr);
			ChordedInputActionWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	UpdateInputActionWidget();
}

void UEnhancedBoundActionButton::UpdateInputActionWidget()
{
	Super::UpdateInputActionWidget();

	if (AssociatedInputAction)
	{
		Text_ActionName->SetText(AssociatedInputAction->ActionDescription);
	}
}
