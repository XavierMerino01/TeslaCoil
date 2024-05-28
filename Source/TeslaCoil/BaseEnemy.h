// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.generated.h"

UCLASS()
class TESLACOIL_API ABaseEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Enemy starting speed when spawned
	UPROPERTY(EditAnywhere)
	float Speed = 100;

	//Value in points that will be added to the player count if this enemy is killed
	UPROPERTY(EditAnywhere)
	float PointValue;

	//Critical range at which the enemy will change default behaviour (basic movement) and attack the player
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange = 300.f;

	//Action to be done by enemies when reaching AttackRange from player (method to be defined in the child enemy class)
	virtual void AttackAction();

	virtual void EnemyMovement(float DeltaTime);

	//Reference to the Player controlled tower
	class AMainTower* MainTower;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	float GetPointValue() const { return PointValue; }

	class UHealthComponent* GetEnemyHealthComponent() const { return EnemyHealth; }

private:

	//Basic components that all enemies have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* EnemyHealth;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UParticleSystem* DeathExplosion;

	//Method to return when the enemy reaches critical distance (determined by AttackRange) from the player and must realize action
	bool InAttackRange();

	//Rotate enemy towards the MainTower 
	void RotateEnemy(FVector LookAtTarget);

	bool bIsAttacking;

};
