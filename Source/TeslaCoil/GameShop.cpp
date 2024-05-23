// Fill out your copyright notice in the Description page of Project Settings.


#include "GameShop.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "BasicGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainTower.h"
#include "BaseEnemy.h"
#include "DefenseTower.h"
#include "HealthComponent.h"
#include "TowerPlayerController.h"
#include "ActorTile.h"
#include "GameStructures.h"
#include "Components/Button.h"

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
	GameStructures = Cast<AGameStructures>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameStructures::StaticClass()));

	InitializeBuyFunctionMap();
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

void AGameShop::UpdateShopOnKill(ABaseEnemy* KilledEnemy)
{
	float EnemyValue = KilledEnemy->GetPointValue();
	UpdatePoints(ShopPoints + EnemyValue);
	if ( EnemyValue == 30)
	{
		KilledBasicEnemies++;
		if (KilledBasicEnemies != 5) return;
		UnlockStructure(0);
	}
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
	
	if (currentHealth < maxHealth && ShopPoints >= HealCost)
	{
		PlayerTowerRef->GetTowerHealthComponent()->HealActor(50);
		UpdatePoints(ShopPoints - HealCost);
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

void AGameShop::BuyFactory()
{
	if (ShopPoints >= FactoryCost)
	{
		UpdatePoints(ShopPoints - FactoryCost);
		GameStructures->ActivateFactory();
		FactoryCost += 100;
		if (CurrentButton != nullptr)
		{
			ManageButtonVisibility();
		}
	}
}

void AGameShop::BuyRadio()
{
	if (ShopPoints >= RadioCost)
	{
		UpdatePoints(ShopPoints - RadioCost);
		GameStructures->ActivateRadio();
		RadioCost += 100;
		if (CurrentButton != nullptr)
		{
			ManageButtonVisibility();
		}
	}
}

void AGameShop::BuyBombDrop()
{
	if (ShopPoints >= BombDropCost)
	{
		UpdatePoints(ShopPoints - BombDropCost);

		FVector SpawnPosition;
		SpawnPosition.X = 150;
		SpawnPosition.Y = -1960;
		SpawnPosition.Z = 1090;
		AActor* NewBomb = GetWorld()->SpawnActor<AActor>(Bomb, SpawnPosition, FRotator::ZeroRotator);
	}
}


void AGameShop::BuyMiniCoil()
{
	if(ShopPoints >= BuyMiniCoilCost)
	{
		PlayerTowerRef->SetControllerToPlaceObject();
		bIsPlacingObject = true;
		NumberOfCoils++;
		if (NumberOfCoils != 2) return;
		UnlockStructure(1);
	}
}

void AGameShop::PlaceNewActor()
{
	if (TargettedTile == nullptr) return;
	if (TargettedTile->GetTileState()) return;

	TargettedTile->SetTileComplete();
	bIsPlacingObject = false;
	FVector SpawnPosition = TargettedTile->GetActorLocation();
	SpawnPosition.Z = 0;
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
	bool bHasTarget = GetWorld()->LineTraceSingleByChannel(MouseHit, Location, End, ECC_GameTraceChannel1, Params);

	if (bHasTarget)
	{
		AActor* HitActor = MouseHit.GetActor();
		if(HitActor && HitActor->IsA<AActorTile>())
		{

			AActorTile* OverlappingTile = Cast<AActorTile>(HitActor);
			return OverlappingTile;
		}
	
		return nullptr;

	}

	return nullptr;

}

void AGameShop::InitializeBuyFunctionMap()
{
	BuyFunctionMap.Add("MiniCoil", &AGameShop::BuyMiniCoil);
	BuyFunctionMap.Add("MaxHealth", &AGameShop::BuyTowerMaxHealth);
	BuyFunctionMap.Add("Repair", &AGameShop::RepairMainTower);
	BuyFunctionMap.Add("Factory", &AGameShop::BuyFactory);
	BuyFunctionMap.Add("Radio", &AGameShop::BuyRadio);
	BuyFunctionMap.Add("MakeItRain", &AGameShop::BuyBombDrop);
}

void AGameShop::ManageButtonVisibility()
{
	CurrentButton->SetVisibility(ESlateVisibility::Collapsed);
	NewUnlockedButton->SetVisibility(ESlateVisibility::Visible);
	CurrentButton = nullptr;
	NewUnlockedButton = nullptr;
}

void AGameShop::BuyAction(FName StructureName, UButton* ClickedButton, UButton* UnlockedButton)
{
	CurrentButton = ClickedButton;
	NewUnlockedButton = UnlockedButton;

	BuyFunction* BuyFunc = BuyFunctionMap.Find(StructureName);
	if (BuyFunc)
	{
		(this->* * BuyFunc)(); // Dereference and call the function
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown structure: %s"), *StructureName.ToString());
	}
}


void AGameShop::UnlockStructure_Implementation(int StructureIndex)
{
	UE_LOG(LogTemp, Error, TEXT("Behaviour of UnlockStructure to be determined in GameShop BP"));
}
