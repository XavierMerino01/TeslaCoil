// Fill out your copyright notice in the Description page of Project Settings.


#include "TeslaTower.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATeslaTower::ATeslaTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	FiringPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	FiringPoint->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void ATeslaTower::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeslaTower::Fire()
{
	//Make sure to check for nullptr in the child classes to avoid crashes (Mainly on TowerPlayerController for MainTower, and CurrentTarget for DefenseTower)
	if (FiringPoint == nullptr) return;
	FVector StartLocation = FiringPoint->GetComponentLocation();

	// Get the hit target as the original end location
	FVector EndLocation = GetTargetLocation();

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

		if (HitActor && HitActor->Tags.Contains("RayTarget"))
		{
			CreateLightningFX(StartLocation, Hit.ImpactPoint, Hit.ImpactNormal);
			OnHitTarget(HitActor); 
		}

	}
	else
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true);
	}
}

FVector ATeslaTower::GetTargetLocation()
{
	return FVector();
}

void ATeslaTower::OnHitTarget(AActor* TargetActor)
{
	if (TargetActor->Tags.Contains("Enemy"))
	{
		AController* MyOwnerInstigator = GetInstigatorController();
		UClass* DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(TargetActor, TowerDamage, MyOwnerInstigator, this, DamageTypeClass);
	}
	else
	{
		MissedHit();
	}
}

void ATeslaTower::MissedHit()
{
	return;
}

void ATeslaTower::CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal)
{
	UE_LOG(LogTemp, Warning, TEXT("LightningFX needs to be implemented in child classes BP"));
}

// Called every frame
void ATeslaTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



