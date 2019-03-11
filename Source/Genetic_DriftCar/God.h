// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Genetic_DriftCar/GeneticAlgorithm/DriftPawn_1HL.h"
#include "AIController.h"
#include "God.generated.h"


struct GeneticConfig {
	int nPopulation;
	float pMutation;
	float pCrossOver;
};

struct CrossProduct {
	TArray<TArray<TArray<float>>> Child1;
	TArray<TArray<TArray<float>>> Child2;
};

UCLASS()
class GENETIC_DRIFTCAR_API AGod : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGod(const FObjectInitializer& ObjectInitializer);
	void Initialize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	void DestroyGeneration();
	void SpawnGeneration(TArray<TArray<TArray<TArray<float>>>> weights);
	TArray<TArray<TArray<TArray<float>>>> CreateMatingPool();
	CrossProduct Cross(TArray<TArray<TArray<TArray<float>>>> matingPool);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawns
	UPROPERTY(Category = "World", BlueprintReadWrite)
		TArray<ADriftPawn_1HL*> DrifterArray;

	TArray<TArray<TArray<float>>> BestChild;
	int HighestFitnessGeneration;
	float HighestFitness;
	

	UPROPERTY(Category = "Genetic", BlueprintReadOnly)
		int GenerationNumber;

	

	DriftPawnData_1HL VConfig;
	GeneticConfig GConfig;

	


	// Config
	UPROPERTY(EditAnywhere, Category = "Spawned Car")
		TSubclassOf<ADriftPawn_1HL> TSubClass_SpawnedCar;

	UPROPERTY(Category = "Config", EditDefaultsOnly)
		FTransform SpawnTransform;

	UPROPERTY(Category = "Config", BlueprintReadWrite, EditDefaultsOnly)
		int NumberOfSensors;

	UPROPERTY(Category = "Config", BlueprintReadWrite, EditDefaultsOnly)
		float SensorViewDistance;

	UPROPERTY(Category = "Config", BlueprintReadWrite, EditDefaultsOnly)
		float DriftScoreMultiplier;


	UPROPERTY(Category = "Genetic", BlueprintReadOnly, EditDefaultsOnly)
		int PopulationSize;

	UPROPERTY(Category = "Genetic", BlueprintReadOnly, EditDefaultsOnly)
		int MutationProbability;

	// Network Config
	UPROPERTY(Category = "Neural Network", BlueprintReadOnly, EditDefaultsOnly)
		TArray<int> HiddenLayerConfig;
};