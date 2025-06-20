// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/OverlayHUD.h"
#include "HUD/UserWidgets/CrosshairWidget.h"

AOverlayHUD::AOverlayHUD()
{
	
}

void AOverlayHUD::BeginPlay()
{
	Super::BeginPlay();

	DisplayOverlayWidget();
}

void AOverlayHUD::DisplayOverlayWidget()
{
	if (OverlayWidget)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidget, FName("Overlay Widget"));
		Widget->AddToViewport();
	}
}
