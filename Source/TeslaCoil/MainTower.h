// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeslaTower.h"
#include "MainTower.generated.h"


/**
 * 
 */
UCLASS()
class TESLACOIL_API AMainTower : public ATeslaTower
{
	GENERATED_BODY()

public:

	AMainTower();
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* GetPlayerController() const { return TowerPlayerController; }

	// Declare the function as a blueprint native event
	UFUNCTION(BlueprintNativeEvent, Category = "MyActor")
	void CreateLightningFX(FVector startPoint, FVector targetPosition, FVector impactNormal);

	// Default implementation of the function
	virtual void CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal);

	UFUNCTION(BlueprintPure)
	float GetEnergyPercent() const;

	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetTowerHealthComponent();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHealthComponent* PlayerTowerHealthComponent;

	void RecoverEnergy();

	void Fire();

	FVector GetHitTarget();

	APlayerController* TowerPlayerController;

	UPROPERTY(EditAnywhere, Category = "Energy")
	float MaxEnergy = 100.f;
	float Energy;

	FTimerHandle EnergyRecoveryTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Energy")
	float EnergyRecoverRate = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Lightning")
	float LightningCost = 10.f;

	
};
