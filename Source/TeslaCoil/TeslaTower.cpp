// Fill out your copyright notice in the Description page of Project Settings.


#include "TeslaTower.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATeslaTower::ATeslaTower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	FiringPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	FiringPoint->SetupAttachment(BaseMesh);

}

// Called when the game starts or when spawned
void ATeslaTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeslaTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



