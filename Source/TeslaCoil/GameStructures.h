// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructures.generated.h"

UCLASS()
class TESLACOIL_API AGameStructures : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameStructures();

	void ActivateFactory();
	void ActivateRadio();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActorTile> ObjectTile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* FactoryMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RadioMesh;

	UPROPERTY(EditAnywhere)
	TArray<FVector> TilePositions;

	void CreateNewTile(int TileIndex);
};
