// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundShooterEnemy.h"
#include "MainTower.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AGroundShooterEnemy::AGroundShooterEnemy()
{
	FiringPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	FiringPoint->SetupAttachment(RootComponent);
}

void AGroundShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AGroundShooterEnemy::AttackAction()
{
	GetWorldTimerManager().SetTimer(EnemyShootingTimerHandle, this, &AGroundShooterEnemy::ShootPlayer, ShootingRate, true);
}

void AGroundShooterEnemy::ShootPlayer()
{

    if (FiringPoint == nullptr) return;
    UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, FiringPoint);

    FVector StartLocation = FiringPoint->GetComponentLocation();


    FVector EndLocation = MainTower->GetActorLocation();

    // Extend the end location by a factor to increase its length
    FVector WorldDirection = (EndLocation - StartLocation).GetSafeNormal(); // Get the direction vector
    float ExtendedLength = (EndLocation - StartLocation).Size() * 2.0f; // Double the length
    EndLocation = StartLocation + (WorldDirection * ExtendedLength); // Extend the end location


    FHitResult Hit;
    FCollisionQueryParams AttackParams;
    AttackParams.AddIgnoredActor(this);
    AttackParams.AddIgnoredActor(GetOwner());
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, AttackParams);

    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor == nullptr) return;

        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, WorldDirection.Rotation());

        AController* MyOwnerInstigator = GetInstigatorController();
        UClass* DamageTypeClass = UDamageType::StaticClass();
        UGameplayStatics::ApplyDamage(HitActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
    }

}

void AGroundShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

