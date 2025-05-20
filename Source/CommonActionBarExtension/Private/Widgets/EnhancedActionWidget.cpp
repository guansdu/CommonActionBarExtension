// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/EnhancedActionWidget.h"

#include "EnhancedInputSubsystems.h"
#include "CommonInputSubsystem.h"
#include "CommonUITypes.h"

FSlateBrush UEnhancedActionWidget::GetIcon() const
{
	if (EnhancedInputAction && CommonUI::IsEnhancedInputSupportEnabled())
	{
		if (const UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			TArray<FKey> BoundKeys = EnhancedInputSubsystem->QueryKeysMappedToAction(EnhancedInputAction);
			FSlateBrush SlateBrush;
	
			const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();
			if (CommonInputSubsystem)
			{
				ECommonInputType InputType = CommonInputSubsystem->GetCurrentInputType();
	
				// Remove the keys that are not applicable to the current input type
				auto KeyFilter = [InputType](const FKey& Key)
				{
					if (InputType == ECommonInputType::Gamepad)
					{
						return !Key.IsGamepadKey();
					}
					if (InputType == ECommonInputType::Touch)
					{
						return !Key.IsTouch();
					}
					if (InputType == ECommonInputType::MouseAndKeyboard)
					{
						return Key.IsGamepadKey() || Key.IsTouch();
					}
					return false;
				};

				BoundKeys.RemoveAll(KeyFilter);
	
				FName GamepadName = CommonInputSubsystem->GetCurrentGamepadName();	
	
				// Pass the key array so that we can get the combined icon brush
				if (!BoundKeys.IsEmpty()
					&& UCommonInputPlatformSettings::Get()->TryGetInputBrush(SlateBrush, BoundKeys, InputType, GamepadName))
				{
					return SlateBrush;
				}
			}
		}
	}

	return Super::GetIcon();
}
