// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeslaTower.h"
#include "DefenseTower.generated.h"

/**
 * 
 */
UCLASS()
class TESLACOIL_API ADefenseTower : public ATeslaTower
{
	GENERATED_BODY()
	
public:

	ADefenseTower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Default implementation of the function
	virtual void CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphere;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckFireCondition();

	virtual void OnHitTarget(AActor* TargetActor) override;

	virtual void MissedHit() override;

	TArray<AActor*> DetectedEnemies;

	virtual void Fire() override;

	AActor* CurrentTarget;

	virtual FVector GetTargetLocation() override;

	void SetEnemyTarget();

	void ResetTarget();

	float FireRate = 1;

	FTimerHandle ShootingTimerHandle;

	bool bHasTarget;

};
