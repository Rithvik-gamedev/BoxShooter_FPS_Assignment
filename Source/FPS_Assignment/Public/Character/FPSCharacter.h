// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"


class AWeaponFPS;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class AProjectileFPS;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdateSignature, float, Score);

UCLASS()
class FPS_ASSIGNMENT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Enhanced Input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//Function to set New score
	void SetNewScore(const float Amount);

	//Delegate to broadcast score when it is updated
	UPROPERTY(BlueprintAssignable)
	FOnScoreUpdateSignature OnScoreUpdate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SpawnAndAttachDefaultWeapon();

	//Fires the projectile from the EquippedWeapon
	void FireWeapon();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Score")
	float Score = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> FPSCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

	//Input Action for Shooting a Weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ShootIA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeaponFPS> DefaultWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AWeaponFPS> EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AProjectileFPS> WeaponProjectile;

public:	

	FORCEINLINE USkeletalMeshComponent* GetCharacterMesh() const { return SkeletalMesh; }
	FORCEINLINE float GetScore() const {return Score; }

};

