// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VTestComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WFUNCCLPS_API UVTestComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVTestComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	bool bTwoDGeneration;

	UPROPERTY(VisibleAnywhere)
	float Gap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PointToSpawn;

	UPROPERTY(EditAnywhere)
	int32 PointsNum;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ToSpawnArray;

	TSubclassOf<AActor> ActorToSpawn;

	void ThreeDimGeneration();
	void TwoDimGeneration();
	void SpawnActorsTwoDim(TArray<FVector> VectorArray, TArray<TArray<bool>> BoolArray);
	void SpawnActorsThreeDim(TArray<FHitResult> VectorArray, TArray<TArray<bool>> BoolArray);
	void CalculateOffsetDirections(TArray<TArray<bool>>& BoolArray, TArray<int32> DirectionArray, int32 SpawnedActorIndex, int32 BoolArrayIndex);
};
