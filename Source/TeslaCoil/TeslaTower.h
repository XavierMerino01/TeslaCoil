// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TeslaTower.generated.h"

UCLASS()
class TESLACOIL_API ATeslaTower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATeslaTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Fire();
	virtual FVector GetTargetLocation();

	virtual void OnHitTarget(AActor* TargetActor);

	virtual void MissedHit();

	// Declare the function as a blueprint native event
	UFUNCTION(BlueprintNativeEvent, Category = "MyActor")
	void CreateLightningFX(FVector startPoint, FVector targetPosition, FVector impactNormal);

	// Default implementation of the function
	virtual void CreateLightningFX_Implementation(FVector startPoint, FVector targetPosition, FVector impactNormal);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Lightning")
	float TowerDamage;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FiringPoint;

};
