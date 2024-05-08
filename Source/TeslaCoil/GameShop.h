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

	AGameShop();

	UFUNCTION(BlueprintCallable)
	void ToggleShopVisibility();

	void StartGameShop();

	UFUNCTION(BlueprintCallable)
	void SetShopWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void RepairMainTower();


private:
	// TO DO: Reference to GameMode as a way to get all info regarding tower: Points, health, energy...
	class ABasicGameMode* GameMode;

    // Reference to the shop UI widget
    UPROPERTY(EditAnywhere)
    class UUserWidget* ShopWidget;

    // Flag to track whether the shop is currently visible
    bool bIsShopVisible = false;

	float MaxHealth;
};
