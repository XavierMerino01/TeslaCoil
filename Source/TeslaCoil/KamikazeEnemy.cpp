// Fill out your copyright notice in the Description page of Project Settings.


#include "KamikazeEnemy.h"
#include "MainTower.h"
#include "Kismet/GameplayStatics.h"

void AKamikazeEnemy::BeginPlay()
{
	Super::BeginPlay();

}

void AKamikazeEnemy::AttackAction()
{
	//TO DO: Apply effects (explosion and sound)
	AController* MyOwnerInstigator = GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(MainTower, Damage, MyOwnerInstigator, this, DamageTypeClass);
	PointValue = 0;
	UGameplayStatics::ApplyDamage(this, Damage, MyOwnerInstigator, this, DamageTypeClass);
}

void AKamikazeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
