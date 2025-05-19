// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnhancedActionBarSubsystem.h"

#include "Utilities/EnhancedActionBarLogs.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedActionKeyMapping.h"

UEnhancedActionBarSubsystem* UEnhancedActionBarSubsystem::Get(const ULocalPlayer* LocalPlayer)
{
	return LocalPlayer ? LocalPlayer->GetSubsystem<UEnhancedActionBarSubsystem>() : nullptr;
}

void UEnhancedActionBarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = Collection.InitializeDependency<UEnhancedInputLocalPlayerSubsystem>();

	EnhancedInputSubsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &UEnhancedActionBarSubsystem::HandleOnControlMappingsRebuilt);
}

void UEnhancedActionBarSubsystem::Deinitialize()
{
	Super::Deinitialize();

	WidgetToActionsMap.Empty();
}

void UEnhancedActionBarSubsystem::RegisterAction(const UUserWidget* Widget, const UInputAction* Action)
{
	if (Widget)
	{
		FWidgetActinsInfo& Result = WidgetToActionsMap.FindOrAdd(MakeWeakObjectPtr(Widget));
		if (Result.Actions.Contains(Action))
		{
			UE_LOG(LogEnhancedActionBar, Warning, TEXT("Failed to register action %s to widget due to already being registered"), *Action->GetName());
		}
		else
		{
			Result.Actions.Add(Action);
			NativeOnActionsUpdated();
		}
	}
}

void UEnhancedActionBarSubsystem::UnregisterAction(const UUserWidget* Widget, const UInputAction* Action)
{
	if (Widget)
	{
		if (WidgetToActionsMap.Contains(MakeWeakObjectPtr(Widget)))
		{
			int32 NumRemoved = WidgetToActionsMap[MakeWeakObjectPtr(Widget)].Actions.Remove(Action);

			if (NumRemoved == 0)
			{
				UE_LOG(LogEnhancedActionBar, Warning, TEXT("Failed to unregister action %s from widget due to not being registered"), *Action->GetName());
			}
			else
			{
				NativeOnActionsUpdated();
			}
		}
	}
}

void UEnhancedActionBarSubsystem::UnregisterAllActions(const UUserWidget* Widget)
{
	if (Widget)
	{
		if (WidgetToActionsMap.Contains(MakeWeakObjectPtr(Widget)))
		{
			WidgetToActionsMap[MakeWeakObjectPtr(Widget)].Actions.Empty();
		}

		WidgetToActionsMap.Remove(MakeWeakObjectPtr(Widget));
	}
}

TArray<const UInputAction*> UEnhancedActionBarSubsystem::GatherAllActions() const
{
	TArray<const UInputAction*> Actions;

	for (auto& Pair : WidgetToActionsMap)
	{
		if (Pair.Key.IsValid())
		{
			for (auto& Action : Pair.Value.Actions)
			{
				if (Action.IsValid())
				{
					Actions.Add(Action.Get());
				}
			}
		}
	}

	return Actions;
}

const UInputAction* UEnhancedActionBarSubsystem::TryFindFirstChordedAction(const UInputAction* Action) const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		TArray<FEnhancedActionKeyMapping> ActionKeyMappings = EnhancedInputSubsystem->GetAllPlayerMappableActionKeyMappings();

		for (const FEnhancedActionKeyMapping& Mapping : ActionKeyMappings)
		{
			if (Mapping.Action == Action)
			{
				for (UInputTrigger* Trigger : Mapping.Triggers)
				{
					if (UInputTriggerChordAction* ChordTrigger = Cast<UInputTriggerChordAction>(Trigger))
					{
						return ChordTrigger->ChordAction;
					}
				}
			}
		}
	}
	UE_LOG(LogEnhancedActionBar, Warning, TEXT("Failed to find chorded action for action %s"), *Action->GetName());
	return nullptr;
}

bool UEnhancedActionBarSubsystem::IsChordedAction(const UInputAction* Action) const
{
	return TryFindFirstChordedAction(Action) != nullptr;
}

void UEnhancedActionBarSubsystem::NativeOnActionsUpdated()
{
	OnActionsUpdated().Broadcast();
}

void UEnhancedActionBarSubsystem::HandleOnControlMappingsRebuilt()
{
	NativeOnActionsUpdated();
}
