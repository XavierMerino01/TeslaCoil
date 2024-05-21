// Fill out your copyright notice in the Description page of Project Settings.


#include "GameShop.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainTower.h"
#include "DefenseTower.h"
#include "HealthComponent.h"
#include "TowerPlayerController.h"
#include "ActorTile.h"

AGameShop::AGameShop()
{
	bIsPlacingObject = false;

	PrimaryActorTick.bCanEverTick = true;
}


void AGameShop::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ABasicGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerController = Cast<ATowerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerTowerRef = Cast<AMainTower>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AGameShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsPlacingObject) return;

	if (TargettedTile != nullptr)
	{
		if (TargettedTile != GetMouseTile())
		{
			TargettedTile->HideTileObject();
			TargettedTile = GetMouseTile();
		}
		if (TargettedTile != nullptr) 
		{
			TargettedTile->ShowTileObject();
		}
	}
	else if (TargettedTile == nullptr)
	{
		TargettedTile = GetMouseTile();
	}
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

void AGameShop::BuyMiniCoil()
{
	if(ShopPoints >= BuyMiniCoilCost)
	{
		PlayerTowerRef->SetControllerToPlaceObject();
		bIsPlacingObject = true;
	}
}

void AGameShop::PlaceNewActor()
{
	if (TargettedTile == nullptr) return;
	if (TargettedTile->GetTileState()) return;

	TargettedTile->SetTileComplete();
	bIsPlacingObject = false;
	FVector SpawnPosition = TargettedTile->GetActorLocation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// Spawn the specified enemy class at the random spawn location
	ADefenseTower* NewTower = GetWorld()->SpawnActor<ADefenseTower>(MiniCoil, SpawnPosition, FRotator::ZeroRotator, SpawnParams);

	UpdatePoints(ShopPoints - BuyMiniCoilCost);
	PlayerTowerRef->SetControllerToAttack();
}



AActorTile* AGameShop::GetMouseTile()
{
	FVector WorldLocation, WorldDirection;

	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	FVector Location = WorldLocation;
	FVector End = WorldLocation + (WorldDirection * 10000.0f);

	FHitResult MouseHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	bool bHasTarget = GetWorld()->LineTraceSingleByChannel(MouseHit, Location, End, ECC_Destructible, Params);


	if (bHasTarget)
	{
		AActor* HitActor = MouseHit.GetActor();
		if(HitActor && HitActor->IsA<AActorTile>())
		{
			UE_LOG(LogTemp, Warning, TEXT("Tile was hit"));

			AActorTile* OverlappingTile = Cast<AActorTile>(HitActor);
			return OverlappingTile;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Hit but not Tile"));
		return nullptr;

	}

	UE_LOG(LogTemp, Warning, TEXT("Tile LOST"));
	return nullptr;

}
