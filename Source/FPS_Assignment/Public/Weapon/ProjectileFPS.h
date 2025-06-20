// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileFPS.generated.h"

class UProjectileMovementComponent;
class ARadialForceActor;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class FPS_ASSIGNMENT_API AProjectileFPS : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileFPS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Callback Function when projectile hits
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Called when DestroyTimer ends
	void DelayedSelfDestroy();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Root")
	TObjectPtr<USceneComponent> ProjectileRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> ParticleEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	//Delay Time to destroy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Destroy Properties")
	float DestroyTime = 5.f;

	//Timer to delay the destruction of projectile when hit
	FTimerHandle DestroyTimer;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
