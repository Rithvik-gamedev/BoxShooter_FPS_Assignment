// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

class AFPSCharacter;
class UWidgetComponent;

UCLASS()
class FPS_ASSIGNMENT_API ABoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sets the BoxColor using DynamicMaterialInstance
	void SetBoxColor(FLinearColor NewColor);

	//Sets the Box properties
	void SetBoxProperties(FString TypeName, FLinearColor Color, int32 InHealth, int32 InScore);

	//Function to Reduce BoxHealth
	void DecrementHealth(const int32 Damage);

	//A BlueprintImplementableEvent to Update BoxHealth to HealthWidget
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Reference to player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Reference")
	TObjectPtr<AFPSCharacter> FPSCharacter;
	
	/*
	 * Box Properties
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Box Properties")
	TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Properties")
	FString BoxName = TEXT("Default");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Properties")
	
	FLinearColor BoxColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Properties")
	int32 BoxHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Properties")
	int32 BoxScore = 0;
	/*
	 * Box Properties
	 */

	//Widget Component to display Box Health
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = " Box Properties")
	TObjectPtr<UWidgetComponent> HealthWidget;
	
};
