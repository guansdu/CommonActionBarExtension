// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnhancedBoundActionBar.h"

#include "Input/CommonUIActionRouterBase.h"
#include "Utilities/EnhancedActionBarSubsystem.h"
#include "Widgets/EnhancedBoundActionButton.h"

void UEnhancedBoundActionBar::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		for (const ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
		{
			if (UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(LocalPlayer))
			{
				EnhancedActionBarSubsystem->OnActionsUpdated().AddUObject(this, &UEnhancedBoundActionBar::HandleOnActionsUpdated);
			}
		}
	}
}

void UEnhancedBoundActionBar::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		for (const ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
		{
			if (UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(LocalPlayer))
			{
				EnhancedActionBarSubsystem->OnActionsUpdated().RemoveAll(this);
			}
		}
	}
}

void UEnhancedBoundActionBar::ActionBarUpdateBeginImpl()
{
	Super::ActionBarUpdateBeginImpl();

	// Be aware that here will remove all action buttons
}

void UEnhancedBoundActionBar::ActionBarUpdateEndImpl()
{
	Super::ActionBarUpdateEndImpl();

	// After the legacy action buttons are built, we can build the enhanced ones
	NativeOnEnhancedActionsUpdated();
}

void UEnhancedBoundActionBar::NativeOnEnhancedActionsUpdated()
{
	UEnhancedActionBarSubsystem* EnhancedActionBarSubsystem = UEnhancedActionBarSubsystem::Get(GetOwningLocalPlayer());
	if (EnhancedActionBarSubsystem)
	{
		TArray<const UInputAction*> Actions = EnhancedActionBarSubsystem->GatherAllActions();
		for (const UInputAction* Action : Actions)
		{
			if (Action)
			{
				if (UEnhancedBoundActionButton* Button = Cast<UEnhancedBoundActionButton>(CreateActionButton(FUIActionBindingHandle())) )
				{
					Button->SetRepresentEnhancedAction(Action);
					NativeOnActionButtonCreated(Button, FUIActionBindingHandle());
				}
			}
		}
	}
}

void UEnhancedBoundActionBar::HandleOnActionsUpdated()
{
	if (!bIsPendingRefresh)
	{
		bIsPendingRefresh = true;
		/***
		 * This is a hacky way to force the action bar to update itself.
		 * We can't override HandleDeferredDisplayUpdate without modifying the source code.
		 */
		if (UCommonUIActionRouterBase* ActionRouter = UCommonUIActionRouterBase::Get(*this))
		{
			ActionRouter->OnBoundActionsUpdated().Broadcast();
		}
	}
}
