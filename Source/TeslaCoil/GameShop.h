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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UUserWidget* ShopWidget;

	void UpdatePoints(float Points);

	void UpdateShopOnKill(class ABaseEnemy* KilledEnemy);

	UFUNCTION(BlueprintCallable)
	float GetCurrentPoints() const;

	UFUNCTION(BlueprintCallable)
	void SetShopWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintNativeEvent, Category = "Game Mode")
	void UnlockStructure(int StructureIndex);

	// Default implementation of the function
	virtual void UnlockStructure_Implementation(int StructureIndex);

	bool RepairMainTower();
	bool BuyTowerMaxHealth();
	bool BuyFactory();
	bool BuyRadio();
	bool BuyBombDrop();
	bool BuyHelicopter();
	bool EndHelicopter();

	UFUNCTION(BlueprintCallable)
	bool BuyAction(FName StructureName, class UButton* ClickedButton, UButton* UnlockedButton);

	UFUNCTION(BlueprintCallable)
	bool BuyMiniCoil();

	void PlaceNewActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ADefenseTower> MiniCoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> Bomb;


private:
	// TO DO: Reference to GameMode as a way to get all info regarding tower: Points, health, energy...
	class ABasicGameMode* GameMode;
	class AMainTower* PlayerTowerRef;
	class ATowerPlayerController* PlayerController;
	class AGameStructures* GameStructures;

	class AActorTile* TargettedTile;
	class AActorTile* GetMouseTile();
	class UButton* CurrentButton;
	UButton* NewUnlockedButton;


	float ShopPoints;

	int KilledBasicEnemies, NumberOfCoils;

	bool bIsPlacingObject;

	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float HealCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float MaxHpCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float FactoryCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float RadioCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float BuyMiniCoilCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float BombDropCost;
	UPROPERTY(EditAnywhere, Category = "Shop Prices")
	float HeliCost;

	UPROPERTY(EditAnywhere, Category = "Shop Objects")
	AActor* Heli;

	float MaxHpCap = 400;

	typedef bool (AGameShop::* BuyFunction)();

	TMap<FName, BuyFunction> BuyFunctionMap;

	void InitializeBuyFunctionMap();

	void ManageButtonVisibility();
};
