// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"
#include "BaseEnemy.h"

// Sets default values
ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawner::SetSpawnerParams(TSubclassOf <ABaseEnemy> EnemyType, int WaveAmount)
{
	CurrentEnemyType = EnemyType;
	EnemyWaveCount = WaveAmount;
}

void ASpawner::StartSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::SpawnEnemy, SpawnInterval, true, 0.0f);
}

void ASpawner::StopSpawner()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLocation = GetActorLocation();
}

void ASpawner::SpawnEnemy()
{
	if (EnemyWaveCount <= 0)
	{
		StopSpawner();
		OnRoundComplete.Broadcast();
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	float RandomX = GetRandomCoordinate(SpawnLocation.X - SpawnRange, SpawnLocation.X + SpawnRange);
	FVector RandomSpawnLocation = FVector(RandomX, SpawnLocation.Y, 50);

	// Spawn the specified enemy class at the random spawn location
	ABaseEnemy* NewEnemy = GetWorld()->SpawnActor<ABaseEnemy>(CurrentEnemyType, RandomSpawnLocation, FRotator::ZeroRotator, SpawnParams);
	EnemyWaveCount--;
}

float ASpawner::GetRandomCoordinate(float MinValue, float MaxValue)
{
	return FMath::RandRange(MinValue, MaxValue);
}
;



