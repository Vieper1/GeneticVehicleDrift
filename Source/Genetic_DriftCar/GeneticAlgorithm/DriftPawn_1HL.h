// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Genetic_DriftCarPawn.h"
#include "DriftPawn_1HL.generated.h"


struct DriftPawnData_1HL {
	int NumberOfSensors;
	float SensorViewDistance;
	float DriftScoreMultiplier;
	int NumberOfHiddenLayerNodes;
	float WeightMag;
};


UCLASS()
class GENETIC_DRIFTCAR_API ADriftPawn_1HL : public AGenetic_DriftCarPawn
{
	GENERATED_BODY()
	

public:
	ADriftPawn_1HL();
	void Initialize(DriftPawnData_1HL config, TArray<TArray<TArray<float>>> dna);


	

	// Config
	float SensorStartDistance;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	DriftPawnData_1HL DrifterData;

	TArray<float > SensorAngleArray;
	TArray<float> SensorArray;
	float SensorCorrectionDist;

	void InitSensorArrays();


	VehicleInputData CalculateSteeringValue();
	float Sigmoid(float val);
	float eValue;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		void OnCheckpointHit(float hitPoint);


public:
	

	
	TArray<TArray<TArray<float>>> WeightsData;
	TArray<float> NodesData;

	void WeightsInitRandom();


	int CheckpointsHit;
	void TimerCheckFitnessIncreasing();
	float LastFitnessScore;
	FTimerHandle FitnessIncreasingTimerHandle;



	



	



	// Results
	UPROPERTY(Category = "Genetic", BlueprintReadWrite)
		bool bGameOver;

	UPROPERTY(Category = "Genetic", BlueprintReadOnly)
		float FitnessScore;

};
