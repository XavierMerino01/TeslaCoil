// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ABaseEnemy;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundComplete);

UCLASS()
class TESLACOIL_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

    void SetSpawnerParams(TSubclassOf <ABaseEnemy> EnemyType, int WaveAmount);

    void StartSpawner();

    void StopSpawner();

    UPROPERTY(BlueprintAssignable, Category = "Spawner")
    FOnRoundComplete OnRoundComplete;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Function to spawn enemies
    void SpawnEnemy();

    int EnemyTypeIndex;
    int EnemyWaveCount;


    float GetRandomCoordinate(float MinValue, float MaxValue);

    // Timer handle for spawning enemies
    FTimerHandle SpawnTimerHandle;

    // Interval between enemy spawns (in seconds)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnInterval;

    // Reference to the enemy to spawn
    TSubclassOf<ABaseEnemy> CurrentEnemyType;

    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    float SpawnRange = 500.0f;

    // Spawn location for enemies
    FVector SpawnLocation;


};
