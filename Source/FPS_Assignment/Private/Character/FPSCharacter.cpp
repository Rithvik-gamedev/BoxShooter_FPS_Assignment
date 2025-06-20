// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/ProjectileFPS.h"
#include "Weapon/WeaponFPS.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(SkeletalMesh, FName("head"));
	SpringArm->TargetArmLength = 0.f;
	
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FPSCamera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Spawn and attach default Weapon at the start of the game
	SpawnAndAttachDefaultWeapon();
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(ShootIA, ETriggerEvent::Triggered, this, &AFPSCharacter::FireWeapon);
	}
}


void AFPSCharacter::FireWeapon()
{
	if (EquippedWeapon)
	{
		const USkeletalMeshSocket* MuzzleSocket = EquippedWeapon->GetWeaponMesh()->GetSocketByName(FName("MuzzleSocket"));
		if (MuzzleSocket && WeaponProjectile)
		{
			const FTransform SocketTransform = MuzzleSocket->GetSocketTransform(EquippedWeapon->GetWeaponMesh());
			
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			GetWorld()->SpawnActor<AProjectileFPS>(WeaponProjectile, SocketTransform, SpawnParameters);
		}
	}
}

void AFPSCharacter::SpawnAndAttachDefaultWeapon()
{
	if (DefaultWeapon)
	{
		const USkeletalMeshSocket* Socket = GetCharacterMesh()->GetSocketByName(FName("RifleSocket"));
		if (Socket)
		{
			AWeaponFPS* SpawnedWeapon = GetWorld()->SpawnActor<AWeaponFPS>(DefaultWeapon);
			if (SpawnedWeapon)
			{
				Socket->AttachActor(SpawnedWeapon, GetCharacterMesh());
				EquippedWeapon = SpawnedWeapon;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Socket is not found."))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Default Weapon Class is not configured in the BP."))
	}
}


void AFPSCharacter::SetNewScore(const float Amount)
{
	Score += Amount;
	//Broadcast the updated score to ScoreWidget
	OnScoreUpdate.Broadcast(Score);
}



