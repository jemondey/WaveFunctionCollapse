// Fill out your copyright notice in the Description page of Project Settings.


#include "VTestComp.h"
#include "RenderCore.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UVTestComp::UVTestComp()
{


}


// Called when the game starts
void UVTestComp::BeginPlay()
{
	Super::BeginPlay();
    

    if (!bTwoDGeneration)
    {
        ThreeDimGeneration();
    }
    else
    {
        TwoDimGeneration();
    }
    
}

void UVTestComp::ThreeDimGeneration()
{
    TArray<FHitResult> VectorArray;
    TArray<TArray<bool>> BoolArray;

    FVector Center;
    FVector Extent;
    FBox Box = GetOwner()->GetStreamingBounds();

    Box = Box.ExpandBy(5.f);
    Center = Box.GetCenter();
    Extent = Box.GetExtent();

    FVector Start = Center - Extent;
    FVector ToSpawnLocation = Start;

    float SideLength = Extent.X * 2;
    Gap = SideLength / (PointsNum - 1);

    FHitResult Hit;

    for (int32 k = 0; k < PointsNum; ++k)
    {
        for (int32 i = 0; i < PointsNum; ++i)
        {
            for (int32 j = 0; j < PointsNum; ++j)
            {
                if (k == 0 || k == PointsNum - 1 || i == 0 || i == PointsNum - 1 || j == 0 || j == PointsNum - 1)
                {
                    FRotator Rotation = UKismetMathLibrary::MakeRotFromX(Center - ToSpawnLocation);
                    FVector Scale(0.5f, 0.5f, 0.5f);
                    FTransform PointTransform(Rotation, ToSpawnLocation, Scale);

                    AActor* Point = GetWorld()->SpawnActor<AActor>(PointToSpawn, PointTransform);
                    bool bHit = GetWorld()->LineTraceSingleByChannel(
                        Hit,
                        ToSpawnLocation,
                        GetOwner()->GetActorLocation(),
                        ECollisionChannel::ECC_Visibility
                    );

                    if (bHit)
                    {
                        VectorArray.Add(Hit);
                        TArray<bool> ObjectsToSpawnArray;
                        ObjectsToSpawnArray.Init(true, ToSpawnArray.Num());
                        BoolArray.Add(ObjectsToSpawnArray);
                    }
                    Point->Destroy();
                }
                ToSpawnLocation.X += Gap;
            }
            ToSpawnLocation.X = Start.X;
            ToSpawnLocation.Y += Gap;
        }
        ToSpawnLocation.X = Start.X;
        ToSpawnLocation.Y = Start.Y;
        ToSpawnLocation.Z += Gap;
    }
    SpawnActorsThreeDim(VectorArray, BoolArray);
}

void UVTestComp::SpawnActorsThreeDim(TArray<FHitResult> VectorArray, TArray<TArray<bool>> BoolArray)
{
    int32 BoolArrayNum = BoolArray.Num();
    int32 ToSpawnArrayNum = ToSpawnArray.Num();
    for (int32 BoolArrayIndex = 0; BoolArrayIndex < BoolArrayNum; ++BoolArrayIndex)
    {
        FVector HitNormal = VectorArray[BoolArrayIndex].ImpactNormal;
        FVector HitPoint = VectorArray[BoolArrayIndex].ImpactPoint;

        TArray<TSubclassOf<AActor>> AcceptableActors;
        for (int32 ToSpawnIndex = 0; ToSpawnIndex < ToSpawnArrayNum; ++ToSpawnIndex)
        {
            bool bIsAcceptable = BoolArray[BoolArrayIndex][ToSpawnIndex];
            if (bIsAcceptable)
            {
                AcceptableActors.Add(ToSpawnArray[ToSpawnIndex]);
            }
        }
        int32 AcceptableActorsNum = AcceptableActors.Num() - 1;
        if (AcceptableActorsNum >= 0)
        {
            int32 ActorSpawnIndex = FMath::RandRange(0, AcceptableActorsNum);
            ActorToSpawn = AcceptableActors[ActorSpawnIndex];
        }
        if (ActorToSpawn)
        {
            int32 SpawnedActorIndex = ToSpawnArray.Find(ActorToSpawn);

            FRotator Rotation;
            AActor* SpawnedCube = GetWorld()->SpawnActor<AActor>(ActorToSpawn, HitPoint, Rotation);

            FVector LocationFromImpact = (HitNormal * 300.f) + HitPoint;
            FVector DirectionToImpact = LocationFromImpact - HitPoint;
            Rotation = UKismetMathLibrary::MakeRotFromZ(DirectionToImpact);
            SpawnedCube->SetActorRotation(Rotation);

            int32 BoolArrayIndexFrwd = BoolArrayIndex + 1;
            int32 BoolArrayIndexBckw = BoolArrayIndex - 1;
            int32 BoolArrayIndexRght = BoolArrayIndex + PointsNum;
            int32 BoolArrayIndexLeft = BoolArrayIndex - PointsNum;
            int32 BoolArrayIndexUp = BoolArrayIndex * PointsNum - 1;

            TArray<int32> DirectionArray;
            DirectionArray.Add(BoolArrayIndexFrwd);
            DirectionArray.Add(BoolArrayIndexBckw);
            DirectionArray.Add(BoolArrayIndexRght);
            DirectionArray.Add(BoolArrayIndexLeft);

            CalculateOffsetDirections(BoolArray, DirectionArray, SpawnedActorIndex, BoolArrayIndex);
        }
    }
}

void UVTestComp::TwoDimGeneration()
{
    TArray<FVector> VectorArray;
    TArray<TArray<bool>> BoolArray;

    FVector Center;
    FVector Extent;
    FBox Box = GetOwner()->GetStreamingBounds();

    Box = Box.ExpandBy(-50.f);
    Center = Box.GetCenter();
    Extent = Box.GetExtent();

    FVector Start = Center - Extent;
    FVector ToSpawnLocation = Start;
    ToSpawnLocation.Z += 100;

    float SideLength = Extent.X * 2;
    Gap = SideLength / (PointsNum - 1);

    FHitResult Hit;

    for (int32 i = 0; i < PointsNum; ++i)
    {
        for (int32 j = 0; j < PointsNum; ++j)
        {
                FRotator rotation(0, 0, 0);
                FVector scale(0.5f, 0.5f, 0.5f);
                FTransform PointTransform(rotation, ToSpawnLocation, scale);

                AActor* Point = GetWorld()->SpawnActor<AActor>(PointToSpawn, PointTransform);
                FVector PointLocation = Point->GetActorLocation();
                FVector UnderSurfaceLocation = PointLocation;
                UnderSurfaceLocation.Z -= 200;
                bool bHit = GetWorld()->LineTraceSingleByChannel(
                    Hit,
                    PointLocation,
                    UnderSurfaceLocation,
                    ECollisionChannel::ECC_Visibility
                );

                if (bHit)
                {
                    VectorArray.Add(Hit.ImpactPoint);
                    TArray<bool> ObjectsToSpawnArray;
                    ObjectsToSpawnArray.Init(true, ToSpawnArray.Num());
                    BoolArray.Add(ObjectsToSpawnArray);

                }
                Point->Destroy();
            ToSpawnLocation.X += Gap;
        }
        ToSpawnLocation.X = Start.X;
        ToSpawnLocation.Y += Gap;
    }

    SpawnActorsTwoDim(VectorArray, BoolArray);
}

void UVTestComp::SpawnActorsTwoDim(TArray<FVector> VectorArray, TArray<TArray<bool>> BoolArray)
{
    int32 BoolArrayNum = BoolArray.Num();
    int32 ToSpawnArrayNum = ToSpawnArray.Num();
    for (int32 BoolArrayIndex = 0; BoolArrayIndex < BoolArrayNum; ++BoolArrayIndex)
    {
        TArray<TSubclassOf<AActor>> AcceptableActors;
        for (int32 ToSpawnIndex = 0; ToSpawnIndex < ToSpawnArrayNum; ++ToSpawnIndex)
        {
            bool bIsAcceptable = BoolArray[BoolArrayIndex][ToSpawnIndex];
            if (bIsAcceptable)
            {
                AcceptableActors.Add(ToSpawnArray[ToSpawnIndex]);
            }
        }
        int32 AcceptableActorsNum = AcceptableActors.Num() - 1;
        if (AcceptableActorsNum >= 0)
        {
            int32 ActorSpawnIndex = FMath::RandRange(0, AcceptableActorsNum);
            ActorToSpawn = AcceptableActors[ActorSpawnIndex];
        }
        if (ActorToSpawn)
        {
            int32 SpawnedActorIndex = ToSpawnArray.Find(ActorToSpawn);

            FRotator Rotation = GetOwner()->GetActorRotation();
            AActor* SpawnedCube = GetWorld()->SpawnActor<AActor>(ActorToSpawn, VectorArray[BoolArrayIndex], Rotation);
            FVector NewLocation = SpawnedCube->GetActorLocation();
            NewLocation.Z = SpawnedCube->GetStreamingBounds().GetExtent().Z + VectorArray[BoolArrayIndex].Z;
            SpawnedCube->SetActorLocation(NewLocation);

            int32 BoolArrayIndexFrwd = BoolArrayIndex + 1;
            int32 BoolArrayIndexRght = BoolArrayIndex + PointsNum;

            TArray<int32> DirectionArray;
            DirectionArray.Add(BoolArrayIndexFrwd);
            DirectionArray.Add(BoolArrayIndexRght);

            CalculateOffsetDirections(BoolArray, DirectionArray, SpawnedActorIndex, BoolArrayIndex);
            
        }
    }
}

void UVTestComp::CalculateOffsetDirections(TArray<TArray<bool>> &BoolArray, TArray<int32> DirectionArray, int32 SpawnedActorIndex, int32 BoolArrayIndex)
{
    for (int32 Direction : DirectionArray)
    {
        if (BoolArrayIndex > 0 && Direction == DirectionArray[0] && Direction % PointsNum == 0.f)
        {
            continue;
        }
        for (int32 ArrayOffset = -2; ArrayOffset <= 2; ArrayOffset += 4)
        {
            int32 BoolChangeBounds = SpawnedActorIndex + ArrayOffset;

            if (BoolArray.IsValidIndex(Direction) && ToSpawnArray.IsValidIndex(BoolChangeBounds))
            {
                switch (SpawnedActorIndex)
                {
                case 0:
                    BoolArray[Direction][BoolChangeBounds] = false;
                    BoolArray[Direction].Last() = false;
                case 3:
                    BoolArray[Direction][BoolChangeBounds] = false;
                    BoolArray[Direction][0] = false;
                default:
                    BoolArray[Direction][BoolChangeBounds] = false;
                }
            }

        }
    }
}
