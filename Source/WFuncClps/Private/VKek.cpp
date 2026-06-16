// Fill out your copyright notice in the Description page of Project Settings.


#include "VKek.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

// Sets default values
AVKek::AVKek()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AVKek::BeginPlay()
{
	Super::BeginPlay();
	
	/*TArray<FVector> Kek = GetMeshVertices(StaticMesh);
	for (FVector k : Kek)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation() + k, 10.f, 12, FColor::Red, true);
	}*/
	TArray<bool> bb;
	TArray<TArray<bool>> br;
	bb.Add(true);
	bb.Add(false);
	br.Add(bb);
	UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (br[0][1] ? TEXT("true") : TEXT("false")));
	
}

void AVKek::Tick(float DeltaTime)
{
	FHitResult Hit;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		GetActorForwardVector() * 10000 + GetActorLocation(),
		ECollisionChannel::ECC_Visibility
	);
	FVector kek = (Hit.ImpactNormal * FVector(300, 300, 300)) + Hit.ImpactPoint;
	DrawDebugLine(GetWorld(), Hit.ImpactPoint, kek, FColor::Green, false);
}

TArray<FVector> AVKek::GetMeshVertices(UStaticMeshComponent* MeshComponent)
{
	TArray<FVector> Vertices;
	if (MeshComponent && MeshComponent->GetStaticMesh())
	{
		FStaticMeshLODResources& LODModel = MeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
		const int32 NumVertices = LODModel.VertexBuffers.PositionVertexBuffer.GetNumVertices();
		Vertices.SetNumUninitialized(NumVertices);
		FMemory::Memcpy(Vertices.GetData(), LODModel.VertexBuffers.PositionVertexBuffer.GetVertexData(), NumVertices * sizeof(FVector));
	}
	return Vertices;
}