// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Genetic_DriftCarPawn.h"
#include "DriftPawn.generated.h"

struct DriftPawnData {
	int NumberOfSensors;
	float SensorViewDistance;
	float DriftScoreMultiplier;
	TArray<int> HiddenLayerConfig;
};

UCLASS()
class GENETIC_DRIFTCAR_API ADriftPawn : public AGenetic_DriftCarPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADriftPawn();
	void Initialize(DriftPawnData config);


	TArray<TArray<TArray<float>>> WeightsData;
	TArray<TArray<float>> NodesData;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	DriftPawnData DrifterData;
	

	TArray <float > SensorAngleArray;
	TArray<float> SensorArray;
	float SensorCorrectionDist;

	void InitSensorArrays();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;











	void WeightsInit0();
	void WeightsInitRandom();







	// Config
	float SensorStartDistance;



	// Results
	UPROPERTY(Category = "Genetic", BlueprintReadOnly)
		bool bGameOver;

	UPROPERTY(Category = "Genetic", BlueprintReadOnly)
		int FitnessScore;
};
