// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainTower.h"
#include "HealthComponent.h"
#include "BaseEnemy.h"
#include "TowerPlayerController.h"
#include "Spawner.h"
#include "Blueprint/UserWidget.h"
#include "GameShop.h"

void ABasicGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void ABasicGameMode::HandleGameStart()
{
	MainTower = Cast<AMainTower>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<ATowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	ShopInstance = Cast<AGameShop>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameShop::StaticClass()));
	EnemySpawner = Cast<ASpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner::StaticClass()));
	EnemySpawner->OnRoundComplete.AddDynamic(this, &ABasicGameMode::HandleRoundComplete);
	WaveOverBP(false);


	if (EnemySpawner)
	{
		SetAndActivateSpawner();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Spawner was not found, make sure it's created in the scene"));
	}
}

void ABasicGameMode::ActorDied(AActor* DeadActor)
{

	if (DeadActor == MainTower) 
	{
		PlayerController->SetPlayerEnabledState(false);
		GameOver(false);
	}
	else if (ABaseEnemy* KilledEnemy = Cast<ABaseEnemy>(DeadActor))
	{
		ShopInstance->UpdatePoints(ShopInstance->GetCurrentPoints() + KilledEnemy->GetPointValue());
		KilledEnemy->HandleDestruction();
		if (!bIsLastWave) return;
		LastWaveEnemyCount--;

		if (LastWaveEnemyCount == 0) 
		{
			PlayerController->SetPlayerEnabledState(false);
			GameOver(true);
		}
	}
}

float ABasicGameMode::GetPointCount() const
{
	return ShopInstance->GetCurrentPoints();
}


float ABasicGameMode::GetRoundTimeLeft() const
{
	return RemainingWaitTime;
}


void ABasicGameMode::ForceNextRound() 
{
	GetWorld()->GetTimerManager().ClearTimer(RoundTimerHandle);
	WaveOverBP(false);
	SetAndActivateSpawner();

	if (currentWave == RoundsInfo.Num() -1)
	{
		bIsLastWave = true;
		LastWaveEnemyCount = RoundsInfo[currentWave].NumEnemiesToSpawn;
	}
}

void ABasicGameMode::OpenShop() 
{
	ShopInstance->ToggleShopVisibility();
}

void ABasicGameMode::CloseShop()
{
	ShopInstance->ToggleShopVisibility();
}

AGameShop* ABasicGameMode::GetShopInstanceRef() 
{
	return ShopInstance;
}





void ABasicGameMode::HandleRoundComplete()
{
	if (currentWave != RoundsInfo.Num() -1)
	{
		currentWave++;
		WaitAndStartNextWave();
		return;
	}
}



void ABasicGameMode::SetAndActivateSpawner()
{
	EnemySpawner->SetSpawnerParams(RoundsInfo[currentWave].EnemyType, RoundsInfo[currentWave].NumEnemiesToSpawn);
	EnemySpawner->StartSpawner();
}

void ABasicGameMode::WaitAndStartNextWave()
{
	RemainingWaitTime = RoundWaitTime;
	WaveOverBP(true);
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle , this, &ABasicGameMode::RoundCountdown, 1, true, 0.0f);
}

void ABasicGameMode::RoundCountdown() 
{
	if (RemainingWaitTime <= 0)
	{
		ForceNextRound();
		return;
	}

	RemainingWaitTime--;
}


//Blueprint Implemented methods
void ABasicGameMode::UIMaxHealthUpdate_Implementation()
{
	UE_LOG(LogTemp, Error, TEXT("UIMaxHealthUpdate needs to be set in BP"));
}

void ABasicGameMode::WaveOverBP_Implementation(bool bIsOver)
{
	UE_LOG(LogTemp, Error, TEXT("Wave over widget needs to be set in BP"));
}

void ABasicGameMode::GameOver_Implementation(bool bWin)
{
	UE_LOG(LogTemp, Error, TEXT("Game over widget needs to be set in BP"));
}
