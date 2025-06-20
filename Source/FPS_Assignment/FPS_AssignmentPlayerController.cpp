// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPS_AssignmentPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "FPS_AssignmentCameraManager.h"

AFPS_AssignmentPlayerController::AFPS_AssignmentPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AFPS_AssignmentCameraManager::StaticClass();
}

void AFPS_AssignmentPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
