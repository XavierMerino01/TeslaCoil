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

    CurrentTarget = nullptr;
    bHasTarget = false;
}


void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
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
    
    SetEnemyTarget();
    Fire();

}

void ADefenseTower::OnHitTarget(AActor* TargetActor)
{

    Super::OnHitTarget(TargetActor);

    ABaseEnemy* CurrentEnemy = Cast<ABaseEnemy>(TargetActor);
    if (CurrentEnemy)
    {
        UHealthComponent* HealthComponent = CurrentEnemy->GetEnemyHealthComponent();
        if (HealthComponent)
        {
            float EnemyHealth = HealthComponent->GetCurrentHealth();
            if (EnemyHealth <= 0)
            {
                bHasTarget = false;
                CurrentTarget = nullptr;
                DetectedEnemies.Remove(TargetActor);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("HealthComponent is null on the enemy: %s"), *CurrentEnemy->GetName());
        }
    }
    else
    {
        ResetTarget();
    }
}

void ADefenseTower::MissedHit()
{
    DetectedEnemies.Remove(CurrentTarget);
    ResetTarget();
}

void ADefenseTower::Fire()
{
    // Get the hit target as the original end location
    if (CurrentTarget == nullptr) return;
    
    Super::Fire();
}

void ADefenseTower::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA<ABaseEnemy>())
    {
        if (!GetWorld()->GetTimerManager().IsTimerActive(ShootingTimerHandle))
        {
            GetWorldTimerManager().SetTimer(ShootingTimerHandle, this, &ADefenseTower::CheckFireCondition, FireRate, true);
        }
        DetectedEnemies.Add(OtherActor);
    }
}


FVector ADefenseTower::GetTargetLocation()
{
    //Returns the location of the currentTarget. Called by Fire() in parent class 
    return CurrentTarget->GetActorLocation();
}

void ADefenseTower::SetEnemyTarget()
{

    // Find the closest enemy
    float MinDistance = FLT_MAX;
    AActor* ClosestEnemy = nullptr;
    FVector TowerLocation = GetActorLocation();

    DetectedEnemies.RemoveAll([](AActor* Actor) { return Actor == nullptr || !Actor->IsValidLowLevel(); });

    if (DetectedEnemies.Num() == 0) 
    {
        ResetTarget();
        return;
    }

    for (AActor* Enemy : DetectedEnemies)
    {
        float Distance = FLT_MAX;
        if (Enemy != nullptr) 
        {
            Distance = FVector::Distance(TowerLocation, Enemy->GetActorLocation());
        }

        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            ClosestEnemy = Enemy;
        }
    }

    if (ClosestEnemy) 
    {
        CurrentTarget = ClosestEnemy;
        bHasTarget = true;
    }
    else
    {
        ResetTarget();
    }
    
}

void ADefenseTower::ResetTarget()
{
    bHasTarget = false;
    CurrentTarget = nullptr;

    if (DetectedEnemies.Num() == 0) 
    {
        GetWorld()->GetTimerManager().ClearTimer(ShootingTimerHandle);
    }
}




void ADefenseTower::CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal)
{
    UE_LOG(LogTemp, Warning, TEXT("Lightning effect needs to be defined in BP"));
}