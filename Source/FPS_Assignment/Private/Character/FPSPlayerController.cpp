// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSPlayerController.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"

void AFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(MoveIA, ETriggerEvent::Triggered, this, &AFPSPlayerController::Move);
		EnhancedInput->BindAction(LookIA, ETriggerEvent::Triggered, this, &AFPSPlayerController::Look);
		EnhancedInput->BindAction(JumpIA, ETriggerEvent::Triggered, this, &AFPSPlayerController::Jump);
	}
}

void AFPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem && MappingContext)
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
}

void AFPSPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D InputVector = Value.Get<FVector2d>();
	const FRotator Rotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputVector.X);
	}
}

void AFPSPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D InputVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerPitchInput(-InputVector.Y);
		ControlledPawn->AddControllerYawInput(InputVector.X);
	}
}

void AFPSPlayerController::Jump(const FInputActionValue& Value)
{
	const bool bInput = Value.Get<bool>();
	APawn* ControlledPawn = GetPawn<APawn>();
	if (bInput && ControlledPawn)
	{
		if (ACharacter* FPSCharacter = Cast<ACharacter>(ControlledPawn))
		{
			FPSCharacter->Jump();
		}
	}
}
