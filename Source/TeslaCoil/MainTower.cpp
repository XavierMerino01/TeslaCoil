// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTower.h"
#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Blueprint/UserWidget.h"


AMainTower::AMainTower()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    PlayerTowerHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

    PrimaryActorTick.bCanEverTick = true;
}



void AMainTower::CreateLightningFX_Implementation(FVector startPoint, FVector impactPoint, FVector impactNormal)
{
    UE_LOG(LogTemp, Error, TEXT("Lightning effect needs to be defined in BP"));
}

float AMainTower::GetEnergyPercent() const
{
    return Energy/MaxEnergy;
}


UHealthComponent* AMainTower::GetTowerHealthComponent()
{
    return PlayerTowerHealthComponent;
}


void AMainTower::BeginPlay()
{
	Super::BeginPlay();

    Energy = MaxEnergy;
    TowerPlayerController = Cast<APlayerController>(GetController());
    PlayerTowerHealthComponent->SetMaxHealth(100);
}


void AMainTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    RecoverEnergy();
}

void AMainTower::RecoverEnergy()
{
    if (Energy < MaxEnergy)
    {
        float EnergyToAdd = EnergyRecoverRate * GetWorld()->GetDeltaSeconds();

        // Add the calculated energy to the current energy level
        Energy += EnergyToAdd;
    }
    else { Energy = MaxEnergy; }
}

void AMainTower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMainTower::Fire);
}


void AMainTower::Fire()
{
    if (Energy < LightningCost) 
    {
        UE_LOG(LogTemp, Error, TEXT("NO ENERGY"));
        return;
    }

    if (FiringPoint == nullptr) return;
    FVector StartLocation = FiringPoint->GetComponentLocation();
    if (TowerPlayerController == nullptr) return;

    // Get the hit target as the original end location
    FVector EndLocation = GetHitTarget();

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
            Energy -= LightningCost;
            CreateLightningFX(StartLocation, Hit.ImpactPoint, Hit.ImpactNormal);
            OnHitTarget(HitActor);
        }

    }
    else 
    {
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true);
    }
}

void AMainTower::OnHitTarget(AActor* TargetActor)
{
    if (TargetActor->Tags.Contains("Enemy"))
    {

        AController* MyOwnerInstigator = GetInstigatorController();
        UClass* DamageTypeClass = UDamageType::StaticClass();
        UGameplayStatics::ApplyDamage(TargetActor, LightningDamage, MyOwnerInstigator, this, DamageTypeClass);
    }
}

FVector AMainTower::GetHitTarget()
{
    FVector WorldLocation, WorldDirection;
    
    TowerPlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
    FVector Location = WorldLocation;
    FVector End = WorldLocation + (WorldDirection * 10000.0f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(GetOwner());
    bool bHasTarget = GetWorld()->LineTraceSingleByChannel(Hit, Location, End,  ECC_Destructible, Params);
    
    if (bHasTarget)
    {
        // If there is a hit, return the hit location
        return Hit.Location;
    }
    else
    {
        // If there is no hit, return a default or invalid location
        return FVector::ZeroVector; // Default to zero vector
    }
}

