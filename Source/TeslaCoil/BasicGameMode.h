// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicGameMode.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRoundInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class ABaseEnemy> EnemyType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 NumEnemiesToSpawn;
};

UCLASS()
class TESLACOIL_API ABasicGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintPure)
	float GetPointCount() const;

	UFUNCTION(BlueprintPure)
	float GetRoundTimeLeft() const;



	UFUNCTION(BlueprintCallable)
	void ForceNextRound();

	UFUNCTION(BlueprintCallable)
	AGameShop* GetShopInstanceRef();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Mode")
	TArray<FRoundInfo> RoundsInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* GameInterfaceWidget;

	UFUNCTION(BlueprintNativeEvent, Category = "Game Mode")
	void UIMaxHealthUpdate();

	virtual void UIMaxHealthUpdate_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Game Mode")
	void WaveOverBP(bool bIsOver);

	// Default implementation of the function
	virtual void WaveOverBP_Implementation(bool bIsOver);

	UFUNCTION(BlueprintNativeEvent, Category = "Game Mode")
	void GameOver(bool bWin);

	// Default implementation of the function
	virtual void GameOver_Implementation(bool bWin);

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleRoundComplete();

	FTimerHandle RoundTimerHandle;

	UPROPERTY()
	class AGameShop* ShopInstance;

private:

	class AMainTower* MainTower;
	class ATowerPlayerController* PlayerController;
	class ASpawner* EnemySpawner;

	UPROPERTY(EditAnywhere)
	int playerScore;

	int currentWave;

	UPROPERTY(EditAnywhere, Category = "Game Mode")
	float RoundWaitTime;

	float RemainingWaitTime;

	void HandleGameStart();

	void SetAndActivateSpawner();

	void WaitAndStartNextWave();

	void RoundCountdown();

	bool bIsLastWave;
	float LastWaveEnemyCount;
	
};
