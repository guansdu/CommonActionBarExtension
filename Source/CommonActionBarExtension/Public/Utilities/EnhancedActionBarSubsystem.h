// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "EnhancedActionBarSubsystem.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class COMMONACTIONBAREXTENSION_API UEnhancedActionBarSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	static UEnhancedActionBarSubsystem* Get(const ULocalPlayer* LocalPlayer);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void RegisterAction(const UUserWidget* Widget, const UInputAction* Action);

	void UnregisterAction(const UUserWidget* Widget, const UInputAction* Action);

	void UnregisterAllActions(const UUserWidget* Widget);

	DECLARE_EVENT(UEnhancedActionBarSubsystem, FOnActionsUpdatedEvent);
	FOnActionsUpdatedEvent& OnActionsUpdated() { return OnActionsUpdatedEvent; }

	TArray<const UInputAction*> GatherAllActions() const;

	const UInputAction* TryFindFirstChordedAction(const UInputAction* Action) const;

	UFUNCTION(BlueprintPure)
	bool IsChordedAction(const UInputAction* Action) const;

protected:
	void NativeOnActionsUpdated();

private:
	struct FWidgetActinsInfo
	{
		TArray<TWeakObjectPtr<const UInputAction> > Actions;
	};

	TMap<TWeakObjectPtr<const UUserWidget>, FWidgetActinsInfo> WidgetToActionsMap;

	mutable FOnActionsUpdatedEvent OnActionsUpdatedEvent;

	UFUNCTION()
	void HandleOnControlMappingsRebuilt();
};
