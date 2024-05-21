// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameShop.generated.h"

/**
 * 
 */
UCLASS()
class TESLACOIL_API AGameShop : public AActor
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	AGameShop();


	void StartGameShop();

	void UpdatePoints(float Points);

	UFUNCTION(BlueprintCallable)
	float GetCurrentPoints() const;

	UFUNCTION(BlueprintCallable)
	void SetShopWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void RepairMainTower();

	UFUNCTION(BlueprintCallable)
	void BuyTowerMaxHealth();

	UFUNCTION(BlueprintCallable)
	void BuyMiniCoil();

	void PlaceNewActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ADefenseTower> MiniCoil;

private:
	// TO DO: Reference to GameMode as a way to get all info regarding tower: Points, health, energy...
	class ABasicGameMode* GameMode;
	class AMainTower* PlayerTowerRef;
	class ATowerPlayerController* PlayerController;

    // Reference to the shop UI widget
    UPROPERTY(EditAnywhere)
    class UUserWidget* ShopWidget;

	class AActorTile* TargettedTile;
	class AActorTile* GetMouseTile();


	float ShopPoints;

	bool bIsPlacingObject;

	UPROPERTY(EditAnywhere)
	float HealCost;
	UPROPERTY(EditAnywhere)
	float MaxHpCost;
	UPROPERTY(EditAnywhere)
	float BuyMiniCoilCost;

	float MaxHpCap = 400;
};
