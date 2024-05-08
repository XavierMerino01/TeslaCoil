// Fill out your copyright notice in the Description page of Project Settings.


#include "GameShop.h"
#include "Blueprint/UserWidget.h"
#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"

AGameShop::AGameShop()
{
	bIsShopVisible = true;
}


void AGameShop::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABasicGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MaxHealth = 100;
}

void AGameShop::StartGameShop()
{
	UE_LOG(LogTemp, Warning, TEXT("Reached gameshop's startgame"));
	//ShopWidget->AddToViewport();
}

void AGameShop::SetShopWidget(UUserWidget* Widget)
{
	ShopWidget = Widget;
}

void AGameShop::RepairMainTower()
{
	float currentHealth = GameMode->GetPlayerHealth();
	
	if (currentHealth < MaxHealth) 
	{
		float PlayerPoints = GameMode->GetPointCount();

		if(PlayerPoints >= 50)
		{
			GameMode->HealPlayer(20);
			GameMode->SetPointCount(PlayerPoints - 50);
		}
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
