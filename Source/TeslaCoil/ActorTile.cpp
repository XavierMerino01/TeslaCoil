// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorTile.h"
#include "DefenseTower.h"

// Sets default values
AActorTile::AActorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = TileBase;

	TileObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileObject"));
	TileObjectMesh->SetupAttachment(RootComponent);
	TileObjectMesh->SetVisibility(false);

	bTileIsUsed = false;
}

// Called when the game starts or when spawned
void AActorTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorTile::ShowTileObject()
{
	if (bTileIsUsed) return;
	if (TileObjectMesh->IsVisible()) return;

	TileObjectMesh->SetVisibility(true);
}

void AActorTile::HideTileObject()
{
	if (!TileObjectMesh->IsVisible()) return;

	TileObjectMesh->SetVisibility(false);
}

void AActorTile::SetTileComplete()
{
	TileObjectMesh->SetVisibility(false);
	bTileIsUsed = true;
}


