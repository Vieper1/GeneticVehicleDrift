// Fill out your copyright notice in the Description page of Project Settings.

#include "DriftPawn_1HL.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "TimerManager.h"
#include "WheeledVehicleMovementComponent4W.h"


ADriftPawn_1HL::ADriftPawn_1HL() {
	GetMesh()->OnComponentHit.AddDynamic(this, &ADriftPawn_1HL::OnCompHit);
	
	SensorStartDistance = 120.f;
	SensorCorrectionDist = 30.f;
	eValue = 1.16f;
}

// Pass empty TArray for dna for new vehicle | updated weights for respawn
void ADriftPawn_1HL::Initialize(DriftPawnData_1HL data, TArray<TArray<TArray<float>>> dna) {
	DrifterData = data;
	FitnessScore = 0.f;
	CheckpointsHit = 0;
	bGameOver = false;
	LastFitnessScore = 0.f;
	InitSensorArrays();
	WeightsInitRandom();

	if (dna.Num() != 0) {
		WeightsData = dna;
	}
}






void ADriftPawn_1HL::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}






void ADriftPawn_1HL::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FitnessIncreasingTimerHandle, this, &ADriftPawn_1HL::TimerCheckFitnessIncreasing, 5.f, true, 5.f);
}




void ADriftPawn_1HL::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameOver) {
		if (GetVelocity().Size2D() > 50.f && DriftZ > 0.15f && DriftZ < 0.6f) {
			FitnessScore += FMath::Abs(DriftZ) * DrifterData.DriftScoreMultiplier;
		}

		// Beam Sensors
		for (int i = 0; i < DrifterData.NumberOfSensors; i++) {
			FHitResult outHit;
			FVector lineStart = GetActorLocation() + (GetActorForwardVector().GetSafeNormal2D() * SensorCorrectionDist) + GetActorForwardVector().GetSafeNormal2D().RotateAngleAxis(SensorAngleArray[i], FVector::UpVector) * SensorStartDistance;
			FVector lineEnd = GetActorLocation() + (GetActorForwardVector().GetSafeNormal2D() * SensorCorrectionDist) + GetActorForwardVector().GetSafeNormal2D().RotateAngleAxis(SensorAngleArray[i], FVector::UpVector) * (SensorStartDistance + DrifterData.SensorViewDistance);
			FCollisionQueryParams collisionParams;

			bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, lineStart, lineEnd, ECC_Visibility, collisionParams);

			if (isHit) {
				if (outHit.Distance < 15.f) bGameOver = true;
				SensorArray[i] = outHit.Distance / DrifterData.SensorViewDistance;
				DrawDebugLine(GetWorld(), lineStart, outHit.Location, FColor::Red, false, 0, 0, 4);
				DrawDebugPoint(GetWorld(), outHit.Location, 32, FColor::Red);
				DrawDebugLine(GetWorld(), outHit.Location, lineEnd, FColor::Green, false, 0, 0, 4);
			} else {
				SensorArray[i] = 1.f;
				DrawDebugLine(GetWorld(), lineStart, lineEnd, FColor::Blue, false, 0, 0, 4);
			}

			

			//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Sensor[%d] = %f"), i, SensorArray[i]));
		}
		// Velocity Sensor
		SensorArray[DrifterData.NumberOfSensors] = GetVelocity().Size2D() / 1000.f;
		//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Sensor[%d] = %f"), DrifterData.NumberOfSensors, SensorArray[DrifterData.NumberOfSensors]));

		// Drift Angle Sensor
		SensorArray[DrifterData.NumberOfSensors + 1] = DriftZ;
		//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Sensor[%d] = %f"), DrifterData.NumberOfSensors + 1, SensorArray[DrifterData.NumberOfSensors + 1]));



		// Use
		if (WeightsData.Num() != 0) {
			VehicleInputData inputData = CalculateSteeringValue();
			/*GEngine->AddOnScreenDebugMessage(-1, 0, FColor::White, FString::Printf(TEXT("%f | %f"), inputData.Steering, inputData.Throttle));*/
			GetVehicleMovementComponent()->SetSteeringInput(inputData.Steering);
			GetVehicleMovementComponent()->SetThrottleInput(/*inputData.Throttle +*/ 1.0f);
		}
		
	} else {
		GetVehicleMovementComponent()->SetSteeringInput(0.f);
		GetVehicleMovementComponent()->SetThrottleInput(0.f);
	}
}

void ADriftPawn_1HL::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Confirmed")));
	if ((OtherActor != NULL) && (OtherActor != this)) {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
	}
}




void ADriftPawn_1HL::OnCheckpointHit(float hitPoint) {
	if (FitnessScore + hitPoint > 0.f) {
		FitnessScore += hitPoint;
	} else {
		FitnessScore = 0.f;
	}
	CheckpointsHit++;
}

void ADriftPawn_1HL::TimerCheckFitnessIncreasing() {
	if (!bGameOver) {
		if (FitnessScore - LastFitnessScore < 100.f) {
			bGameOver = true;
		}
	}
}













//////////////////////////////////////////////////////////////////////////////// Init
void ADriftPawn_1HL::InitSensorArrays() {
	float delta = 240.f / DrifterData.NumberOfSensors;
	for (int i = 0; i < DrifterData.NumberOfSensors; i++) {
		SensorAngleArray.Add(delta * i - 90.f);
	}

	SensorArray.Init(0.f, DrifterData.NumberOfSensors + 2);
}


// WeightsArray[i][j]
// i = Input Node Number
// j = Output Node Number
void ADriftPawn_1HL::WeightsInitRandom() {
	NodesData.Init(0.f, DrifterData.NumberOfHiddenLayerNodes);

	TArray<float> row;
	TArray<TArray<float>> col;

	// Weight Array for Layers Input-HL
	row.Init(0.f, DrifterData.NumberOfHiddenLayerNodes);
	col.Init(row, DrifterData.NumberOfSensors + 2);
	WeightsData.Add(col);

	for (int i = 0; i < col.Num(); i++) {
		for (int j = 0; j < row.Num(); j++) {
			WeightsData[0][i][j] = FMath::RandRange(-1.f * DrifterData.WeightMag, 1.f * DrifterData.WeightMag);
		}
	}

	// Weight Array for Layers HL-Output
	row.Init(0.f, 2);
	col.Init(row, DrifterData.NumberOfHiddenLayerNodes);
	WeightsData.Add(col);

	for (int i = 0; i < col.Num(); i++) {
		for (int j = 0; j < row.Num(); j++) {
			WeightsData[1][i][j] = FMath::RandRange(-1.f * DrifterData.WeightMag, 1.f * DrifterData.WeightMag);
		}
	}
}









//////////////////////////////////////////////////////////////////////////////// Neural Network Usage
VehicleInputData ADriftPawn_1HL::CalculateSteeringValue() {
	VehicleInputData inputData;
	for (int i = 0; i < NodesData.Num(); i++) {
		float sum = 0.f;
		for (int j = 0; j < WeightsData[0].Num(); j++) {
			sum += SensorArray[j] * WeightsData[0][j][i];
		}
		NodesData[i] = sum;
	}

	for (int i = 0; i < 2; i++) {
		float sum = 0.f;
		for (int j = 0; j < WeightsData[1].Num(); j++) {
			sum += NodesData[j] * WeightsData[1][j][i];
		}
		if (i == 0) {
			inputData.Steering = /*Sigmoid(*/sum/*)*/;
			/*inputData.Steering = FMath::GetMappedRangeValueClamped(FVector2D(-1.f * DrifterData.NumberOfHiddenLayerNodes, 1.f * DrifterData.NumberOfHiddenLayerNodes), FVector2D(-1.f, 1.f), sum);*/
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, FString::Printf(TEXT("Vehicle Input [Steering] = %.2f"), sum));
		} else if (i == 1) {
			inputData.Throttle = Sigmoid(sum);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Vehicle Input[Throttle] = %.2f"), sum));
		}
	}


	return inputData;
}

float ADriftPawn_1HL::Sigmoid(float val) {
	float retValNumerator = 1.f - FMath::Pow(eValue, val);
	float retValDenominator = 1.f + FMath::Pow(eValue, val);

	return retValNumerator / retValDenominator;
}










