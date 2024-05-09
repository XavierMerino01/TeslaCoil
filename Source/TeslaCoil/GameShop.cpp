// Fill out your copyright notice in the Description page of Project Settings.


#include "GameShop.h"
#include "Blueprint/UserWidget.h"
#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainTower.h"
#include "HealthComponent.h"

AGameShop::AGameShop()
{
	bIsShopVisible = true;
}


void AGameShop::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABasicGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerTowerRef = Cast<AMainTower>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AGameShop::StartGameShop()
{
	UE_LOG(LogTemp, Warning, TEXT("Reached gameshop's startgame"));
	//ShopWidget->AddToViewport();
}

void AGameShop::UpdatePoints(float Points)
{
	ShopPoints = Points;
}

float AGameShop::GetCurrentPoints() const
{
	return ShopPoints;
}

void AGameShop::SetShopWidget(UUserWidget* Widget)
{
	ShopWidget = Widget;
}

void AGameShop::RepairMainTower()
{
	float currentHealth = PlayerTowerRef->GetTowerHealthComponent()->GetCurrentHealth();;
	float maxHealth = PlayerTowerRef->GetTowerHealthComponent()->GetMaxHealth();
	
	if (currentHealth < maxHealth) 
	{
		if(ShopPoints >= HealCost)
		{
			PlayerTowerRef->GetTowerHealthComponent()->HealActor(50);
			UpdatePoints(ShopPoints - HealCost);
		}
	} 
}

void AGameShop::BuyTowerMaxHealth()
{
	float maxHealth = PlayerTowerRef->GetTowerHealthComponent()->GetMaxHealth();
	if (maxHealth < MaxHpCap && ShopPoints >= MaxHpCost) 
	{
		UpdatePoints(ShopPoints - MaxHpCost);
		PlayerTowerRef->GetTowerHealthComponent()->SetMaxHealth(maxHealth + 100);
		GameMode->UIMaxHealthUpdate();
	}
}

void AGameShop::ToggleShopVisibility()
{
	bIsShopVisible = !bIsShopVisible;

	if (ShopWidget) 
	{
		if (bIsShopVisible) 
		{
			ShopWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ShopWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
}
