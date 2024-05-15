// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseTower.h"
#include "Components/SphereComponent.h"
#include "BaseEnemy.h"
#include "HealthComponent.h"

ADefenseTower::ADefenseTower()
{
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetupAttachment(RootComponent);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADefenseTower::OnOverlapBegin);

    bHasTarget = false;
}


void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();

    GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ADefenseTower::CheckFireCondition, FireRate, true);
}

void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ADefenseTower::CheckFireCondition()
{

    if (DetectedEnemies.Num() == 0) return;

    if (bHasTarget)
    {
        Fire();
        return;
    }
    
    CurrentTarget = GetEnemyTarget();
    bHasTarget = true;
    Fire();

}

void ADefenseTower::OnHitTarget(AActor* TargetActor)
{
    Super::OnHitTarget(TargetActor);

    ABaseEnemy* CurrentEnemy = Cast<ABaseEnemy>(TargetActor);
    float EnemyHealth = CurrentEnemy->GetEnemyHealthComponent()->GetCurrentHealth();
    if (EnemyHealth <= 0) 
    {
        bHasTarget = false;
        DetectedEnemies.Remove(TargetActor);
    }
}

void ADefenseTower::Fire()
{
    if (FiringPoint == nullptr) return;
    FVector StartLocation = FiringPoint->GetComponentLocation();

    // Get the hit target as the original end location
    FVector EndLocation = CurrentTarget;

    // Extend the end location by a factor to increase its length
    FVector WorldDirection = (EndLocation - StartLocation).GetSafeNormal(); // Get the direction vector
    float ExtendedLength = (EndLocation - StartLocation).Size() * 2.0f; // Double the length
    EndLocation = StartLocation + (WorldDirection * ExtendedLength); // Extend the end location


    FHitResult Hit;
    FCollisionQueryParams LightAttackParams;
    LightAttackParams.AddIgnoredActor(this);
    LightAttackParams.AddIgnoredActor(GetOwner());
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Destructible, LightAttackParams);


    if (bHit)
    {
        AActor* HitActor = Hit.GetActor();
        if (HitActor == nullptr) return;

        if (HitActor->Tags.Contains("RayTarget"))
        {
            CreateLightningFX(StartLocation, Hit.ImpactPoint, Hit.ImpactNormal);
            OnHitTarget(HitActor); //Will call parent declared method OnHitTarget() to deal TowerDamage amount to the HitActor health
        }

    }
    else
    {
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true);
    }
}

void ADefenseTower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA<ABaseEnemy>())
    {
        DetectedEnemies.Add(OtherActor);
    }
}


FVector ADefenseTower::GetEnemyTarget()
{

    // Find the closest enemy
    float MinDistance = FLT_MAX;
    AActor* ClosestEnemy = nullptr;
    FVector TowerLocation = GetActorLocation();

    for (AActor* Enemy : DetectedEnemies)
    {
        float Distance = FVector::Distance(TowerLocation, Enemy->GetActorLocation());
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            ClosestEnemy = Enemy;
        }
    }

    return ClosestEnemy->GetActorLocation();
}




void ADefenseTower::CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal)
{
    UE_LOG(LogTemp, Warning, TEXT("Lightning effect needs to be defined in BP"));
}