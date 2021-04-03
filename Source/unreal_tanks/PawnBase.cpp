// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "ProjectileBase.h"
#include "HealthComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void APawnBase::RotateTurret(FVector LookAtTarget)
{
	// Update TurretMesh rotation to face towards the LookAtTarget passed in from Child Classes.
	// TurretMesh->SetWorldRotation()...
	FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
	FVector StartLocation = TurretMesh->GetComponentLocation();

	FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
	TurretMesh->SetWorldRotation(TurretRotation);

}

void APawnBase::Fire()
{
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at Location firing towards Rotation.
	// Get ProjectileSpawnPoint Location && Rotation -> Spawn Projectile class at Location firing towards Rotation.
	if (ProjectileClass)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
		TempProjectile->SetOwner(this);
	}

}

void APawnBase::HandleDestruction()
{
	// --- Universal functionality --- 
	// Play death effects particle, sound and camera shake.
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);

	// --- Then do Child overrides ---
	// -- PawnTurret - Inform GameMode Turret died -> Then Destroy() self. 

	// -- PawnTank - Inform GameMode Player died -> Then Hide() all components && stop movement input.
}


//// Called when the game starts or when spawned
//void APawnBase::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void APawnBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

