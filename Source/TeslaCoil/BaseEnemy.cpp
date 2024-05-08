// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "MainTower.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(BoxComp);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	MainTower = Cast<AMainTower>(UGameplayStatics::GetPlayerPawn(this, 0));
	RotateEnemy(MainTower->GetActorLocation());
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InAttackRange() && !bIsAttacking)
	{
		bIsAttacking = true;
		AttackAction();
	}
	else if (!bIsAttacking)
	{
		EnemyMovement(DeltaTime);
	}

}

void ABaseEnemy::EnemyMovement(float DeltaTime)
{
	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Speed * DeltaTime;
	AddActorLocalOffset(DeltaLocation, true);
}

void ABaseEnemy::AttackAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Attack action needs to be defined in children inheriting class"));
}

void ABaseEnemy::RotateEnemy(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - GetActorLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	SetActorRotation(LookAtRotation);
}


void ABaseEnemy::HandleDestruction()
{
	Destroy();
}

bool ABaseEnemy::InAttackRange()
{
	if (MainTower)
	{
		float Distance = FVector::Dist(GetActorLocation(), MainTower->GetActorLocation());
		if (Distance <= AttackRange)
		{
			return true;
		}
	}
	return false;
}



