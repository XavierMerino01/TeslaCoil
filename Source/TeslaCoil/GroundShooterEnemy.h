// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "GroundShooterEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TESLACOIL_API AGroundShooterEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	// Sets default values for this pawn's properties
	AGroundShooterEnemy();

protected:

	virtual void BeginPlay() override;

	virtual void AttackAction() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle EnemyShootingTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FiringPoint;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;


	UPROPERTY(EditAnywhere)
	float Damage = 10;

	float ShootingRate = 1;

	void ShootPlayer();

};
