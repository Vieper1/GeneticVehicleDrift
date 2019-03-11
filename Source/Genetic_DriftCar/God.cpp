// Fill out your copyright notice in the Description page of Project Settings.

#include "God.h"
#include "Engine/Engine.h"
#include "Components/ArrowComponent.h"
#include "AIController.h"
#include "Genetic_DriftCar/GeneticAlgorithm/DriftPawn_1HL.h"



// Sets default values
AGod::AGod(const FObjectInitializer& ObjectInitializer) {
	PrimaryActorTick.bCanEverTick = true;
	
	UArrowComponent* arrow = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, FName("Forward Arrow"));
	arrow->bHiddenInGame = false;
	arrow->Mobility = EComponentMobility::Movable;
	arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = arrow;

	Initialize();
}

void AGod::Initialize() {
	VConfig.NumberOfSensors = 4;
	VConfig.SensorViewDistance = 10000.f;
	VConfig.DriftScoreMultiplier = 5.f;
	VConfig.NumberOfHiddenLayerNodes = 6;
	VConfig.WeightMag = 5.f;

	GConfig.nPopulation = 20;		// Even values only
	GConfig.pCrossOver = 0.5f;
	GConfig.pMutation = 0.005f;
}

// Called when the game starts or when spawned
void AGod::BeginPlay() {
	Super::BeginPlay();

	SpawnGeneration(TArray<TArray<TArray<TArray<float>>>>());
}

// Called every frame
void AGod::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("Best Fitness = Gen[%d] | %f"), HighestFitnessGeneration, HighestFitness));

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Generation Number => %d"), GenerationNumber));

	bool bCurrentGenEnded = true;
	for (int i = 0; i < GConfig.nPopulation; i++) {
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Vehicle[%d] (bGameOver,Fitness,CheckPoints) => [ %d \t %0.f \t %d ]"), i, DrifterArray[i]->bGameOver, DrifterArray[i]->FitnessScore, DrifterArray[i]->CheckpointsHit));
		if (!DrifterArray[i]->bGameOver) {
			bCurrentGenEnded = false;
		}
	}

	// Evolve & Reset Vehicles
	if (bCurrentGenEnded) {
		GenerationNumber++;
		TArray<TArray<TArray<TArray<float>>>> MatingPool;
		MatingPool = CreateMatingPool();
		UE_LOG(LogTemp, Warning, TEXT("Mating Pool Size = %d"), MatingPool.Num());


		for (int i = 0; i < DrifterArray.Num(); i++) {
			if (HighestFitness < DrifterArray[i]->FitnessScore) {
				HighestFitness = DrifterArray[i]->FitnessScore;
				BestChild = DrifterArray[i]->WeightsData;
				HighestFitnessGeneration = GenerationNumber;
			}
		}
		

		TArray<TArray<TArray<TArray<float>>>> NextGen;
		while (NextGen.Num() < GConfig.nPopulation) {
			GConfig.pCrossOver = FMath::RandRange(0.f, 1.f);
			CrossProduct cross = Cross(MatingPool);
			NextGen.Add(cross.Child1);
			NextGen.Add(cross.Child2);
		}

		/*for (int i = 0; i < NextGen.Num(); i++) {
			UE_LOG(LogTemp, Warning, TEXT("Log\n--------------------------------------------------\nVehicle Number => %d\n--------------------------------------------------"), i + 1);
			for (int j = 0; j < NextGen[i].Num(); j++) {
				UE_LOG(LogTemp, Warning, TEXT("Layers [%d,%d]\n------------------------------"), j, j+1);
				for (int k = 0; k < NextGen[i][j].Num(); k++) {
					for (int l = 0; l < NextGen[i][j][k].Num(); l++) {
						UE_LOG(LogTemp, Warning, TEXT("Weight[%d][%d] => %.2f"), k, l, NextGen[i][j][k][l]);
					}
					UE_LOG(LogTemp, Warning, TEXT("----------"));
				}
			}
		}*/
		

		DestroyGeneration();
		SpawnGeneration(NextGen);
		//SpawnGeneration(TArray<TArray<TArray<TArray<float>>>>());
	}
}

void AGod::DestroyGeneration() {
	for (int i = 0; i < GConfig.nPopulation; i++) {
		DrifterArray[i]->Destroy();
	}
	DrifterArray.Empty();
}

void AGod::SpawnGeneration(TArray<TArray<TArray<TArray<float>>>> weights) {
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < GConfig.nPopulation; i++) {
		DrifterArray.Add(GetWorld()->SpawnActor<ADriftPawn_1HL>(TSubClass_SpawnedCar, GetActorLocation(), GetActorRotation(), spawnParams));
		if (weights.Num() == 0) {
			DrifterArray[i]->Initialize(VConfig, TArray<TArray<TArray<float>>>());
		} else {
			DrifterArray[i]->Initialize(VConfig, weights[i]);
		}

		AAIController* aiController = GetWorld()->SpawnActor<AAIController>(FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		aiController->Possess(DrifterArray[i]);
	}
}



// 1. Create a mating pool from existing population and individual fitnesses
TArray<TArray<TArray<TArray<float>>>> AGod::CreateMatingPool() {
	TArray<TArray<TArray<TArray<float>>>> pool;
	float sum = 0.f;
	for (int i = 0; i < DrifterArray.Num(); i++) {
		sum += DrifterArray[i]->FitnessScore;
	}

	TArray<int> fitnessArray;
	fitnessArray.Init(0, DrifterArray.Num());

	for (int i = 0; i < DrifterArray.Num(); i++) {
		fitnessArray[i] = FMath::RoundToInt(DrifterArray[i]->FitnessScore / sum * 100.f);
		for (int count = 0; count < fitnessArray[i]; count++) {
			pool.Add(DrifterArray[i]->WeightsData);
		}
	}

	return pool;
}

// 2. Crossing
CrossProduct AGod::Cross(TArray<TArray<TArray<TArray<float>>>> pool) {
	TArray<TArray<TArray<float>>> dna1 = pool[FMath::RandRange(0, pool.Num() - 1)];
	TArray<TArray<TArray<float>>> dna2 = pool[FMath::RandRange(0, pool.Num() - 1)];

	for (int i = 0; i < dna1.Num(); i++) {
		for (int j = 0; j < dna1[i].Num(); j++) {
			for (int k = 0; k < dna1[i][j].Num(); k++) {
				float randNo = FMath::RandRange(0.f, 1.f);
				if (randNo < GConfig.pCrossOver) {
					float temp = dna1[i][j][k];
					dna1[i][j][k] = dna2[i][j][k];
					dna2[i][j][k] = temp;
				}
				if (randNo < GConfig.pMutation) {
					UE_LOG(LogTemp, Warning, TEXT("Mutation!"));
					dna1[i][j][k] = FMath::RandRange(-1.f * VConfig.WeightMag, 1.f * VConfig.WeightMag);
					dna2[i][j][k] = FMath::RandRange(-1.f * VConfig.WeightMag, 1.f * VConfig.WeightMag);
				}
			}
		}
	}

	CrossProduct cross;
	cross.Child1 = dna1;
	cross.Child2 = dna2;

	return cross;
}