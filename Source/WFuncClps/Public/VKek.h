// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VKek.generated.h"

UCLASS()
class WFUNCCLPS_API AVKek : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVKek();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	TArray<FVector> GetMeshVertices(UStaticMeshComponent* MeshComponent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

};