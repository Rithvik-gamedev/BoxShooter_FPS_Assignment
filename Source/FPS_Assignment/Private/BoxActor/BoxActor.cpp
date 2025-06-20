// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxActor/BoxActor.h"

#include "Character/FPSCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABoxActor::ABoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Mesh"));
	SetRootComponent(BoxMesh);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Widget"));
	HealthWidget->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ABoxActor::BeginPlay()
{
	Super::BeginPlay();

	FPSCharacter = Cast<AFPSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (BoxMesh && BoxMesh->GetMaterial(0))
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BoxMesh->GetMaterial(0), this);
		if (DynamicMaterialInstance)
		{
			BoxMesh->SetMaterial(0, DynamicMaterialInstance);
			DynamicMaterialInstance->SetVectorParameterValue(FName("Box Color Param"), BoxColor);
			SetBoxColor(BoxColor);
		}
	}
	
}

// Called every frame
void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoxActor::SetBoxColor(FLinearColor NewColor)
{
	BoxColor = NewColor;
	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue(FName("Box Color Param"), BoxColor);
	}
	else
	{
		if (BoxMesh && BoxMesh->GetMaterial(0))
		{
			DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BoxMesh->GetMaterial(0), this);
			if (DynamicMaterialInstance)
			{
				BoxMesh->SetMaterial(0, DynamicMaterialInstance);
				DynamicMaterialInstance->SetVectorParameterValue(FName("Box Color Param"), BoxColor);
			}
		}
	}
	
}

void ABoxActor::SetBoxProperties(FString TypeName, FLinearColor Color, int32 InHealth, int32 InScore)
{
	BoxName = TypeName;
	SetBoxColor(Color);
	BoxHealth = InHealth;
	BoxScore = InScore;
}

void ABoxActor::DecrementHealth(const int32 Damage)
{
	if (BoxHealth <= 1.f)
	{
		BoxHealth = 0.f;
		if (FPSCharacter)
		{
			FPSCharacter->SetNewScore(BoxScore);
		}
		Destroy();
	}
	else
	{
		BoxHealth -= Damage;
		UpdateHealth();
		
	}
}




