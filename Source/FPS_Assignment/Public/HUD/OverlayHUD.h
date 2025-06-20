// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OverlayHUD.generated.h"

class UCrosshairWidget;
/**
 * 
 */
UCLASS()
class FPS_ASSIGNMENT_API AOverlayHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	AOverlayHUD();
	
protected:

	virtual void BeginPlay() override;


	void DisplayOverlayWidget();
	
	// BP Class to display the OverlayWidget
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overlay")
	TSubclassOf<UUserWidget> OverlayWidget;
};
