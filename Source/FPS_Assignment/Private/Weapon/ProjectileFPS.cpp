// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileFPS.h"
#include "NiagaraComponent.h"
#include "BoxActor/BoxActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectileFPS::AProjectileFPS()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ProjectileRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Root"));
	//SetRootComponent(ProjectileRootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	//Sphere->SetupAttachment(GetRootComponent());
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sphere->SetCollisionResponseToAllChannels(ECR_Block);
	Sphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	ParticleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particle Effect"));
	ParticleEffect->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->bShouldBounce = true;

}

// Called when the game starts or when spawned
void AProjectileFPS::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentHit.AddDynamic(this, &AProjectileFPS::OnComponentHit);
	
}

void AProjectileFPS::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	ABoxActor* HitActor = Cast<ABoxActor>(OtherActor);
	if (HitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHit Box"))
		HitActor->DecrementHealth(1);
		
		if (GetWorldTimerManager().TimerExists(DestroyTimer))
		{
			GetWorldTimerManager().ClearTimer(DestroyTimer);
		}
	}
	if (GetWorldTimerManager().TimerExists(DestroyTimer)) return;
	
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectileFPS::DelayedSelfDestroy, DestroyTime);
	
}

void AProjectileFPS::DelayedSelfDestroy()
{
	Destroy();
}

// Called every frame
void AProjectileFPS::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

