// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "KamikazeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TESLACOIL_API AKamikazeEnemy : public ABaseEnemy
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void AttackAction() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	float Damage = 10;
	
};
