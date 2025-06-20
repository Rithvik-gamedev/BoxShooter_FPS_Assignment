// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponFPS.h"

// Sets default values
AWeaponFPS::AWeaponFPS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(WeaponRootComponent);
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AWeaponFPS::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponFPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

