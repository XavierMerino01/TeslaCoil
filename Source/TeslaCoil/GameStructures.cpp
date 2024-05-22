// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStructures.h"
#include "ActorTile.h"

// Sets default values
AGameStructures::AGameStructures()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FactoryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Factory"));
	FactoryMesh->SetupAttachment(RootComponent);

	RadioMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Radio"));
	RadioMesh->SetupAttachment(RootComponent);

	FactoryMesh->SetVisibility(false);
	RadioMesh->SetVisibility(false);
}

void AGameStructures::ActivateFactory()
{
	FactoryMesh->SetVisibility(true);
	CreateNewTile(0);
}

void AGameStructures::ActivateRadio()
{
	RadioMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AGameStructures::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGameStructures::CreateNewTile(int TileIndex)
{
	FVector SpawnPosition = TilePositions[TileIndex];
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActorTile* NewTile = GetWorld()->SpawnActor<AActorTile>(ObjectTile, SpawnPosition, FRotator::ZeroRotator, SpawnParams);
}

