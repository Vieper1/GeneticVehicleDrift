// Fill out your copyright notice in the Description page of Project Settings.

#include "DriftPawn.h"
#include "Genetic_DriftCarWheelFront.h"
#include "Genetic_DriftCarWheelRear.h"
#include "Genetic_DriftCarHud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "GameFramework/Controller.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADriftPawn::ADriftPawn()
{

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	


	// Debug
	DriftPawnData config;
	config.NumberOfSensors = 20;
	config.SensorViewDistance = 10000.f;
	config.DriftScoreMultiplier = 5.f;
	config.HiddenLayerConfig.Add(5);
	config.HiddenLayerConfig.Add(4);
	Initialize(config);
}

void ADriftPawn::Initialize(DriftPawnData data) {
	DrifterData = data;
	SensorStartDistance = 120.f;
	SensorCorrectionDist = 30.f;

	InitSensorArrays();
}



void ADriftPawn::BeginPlay() {
	Super::BeginPlay();
}
void ADriftPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ADriftPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameOver) {
		FitnessScore += GetVelocity().Size2D() + (FMath::Abs(DriftZ) * DrifterData.DriftScoreMultiplier);
	}

	for (int i = 0; i < DrifterData.NumberOfSensors; i++) {
		FHitResult outHit;
		FVector lineStart = GetActorLocation() + (GetActorForwardVector().GetSafeNormal2D() * SensorCorrectionDist) + GetActorForwardVector().GetSafeNormal2D().RotateAngleAxis(SensorAngleArray[i], FVector::UpVector) * SensorStartDistance;
		FVector lineEnd = GetActorLocation() + (GetActorForwardVector().GetSafeNormal2D() * SensorCorrectionDist) + GetActorForwardVector().GetSafeNormal2D().RotateAngleAxis(SensorAngleArray[i], FVector::UpVector) * (SensorStartDistance + DrifterData.SensorViewDistance);
		FCollisionQueryParams collisionParams;

		bool isHit = GetWorld()->LineTraceSingleByChannel(outHit, lineStart, lineEnd, ECC_Visibility, collisionParams);
		
		if (isHit) {
			SensorArray[i] = outHit.Distance;
			DrawDebugLine(GetWorld(), lineStart, outHit.Location, FColor::Green, false, 0, 0, 2);
			DrawDebugPoint(GetWorld(), outHit.Location, 32, FColor::Green);
			DrawDebugLine(GetWorld(), outHit.Location, lineEnd, FColor::Cyan, false, 0, 0, 2);
		} else {
			SensorArray[i] = DrifterData.SensorViewDistance;
			DrawDebugLine(GetWorld(), lineStart, lineEnd, FColor::Red, false, 0, 0, 2);
		}

		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("Sensor[%d] = %f"), i + 1, SensorArray[i]));
	}
}






void ADriftPawn::InitSensorArrays() {
	int delta = 360 / DrifterData.NumberOfSensors;
	for (int i = 0; i < DrifterData.NumberOfSensors; i++) {
		SensorAngleArray.Add(delta * i);
	}
	SensorArray.Init(0.f, DrifterData.NumberOfSensors + 2);
}



void ADriftPawn::WeightsInit0() {
	/*TArray<float> nodeUnit;
	for (int i=0; i<Drifter)
	nodeUnit.Init(0, DrifterData.HiddenLayerConfig)

	TArray<float> row;
	TArray<TArray<float>> col;

	

	for (int i = 0; i < DrifterData.HiddenLayerConfig.Num(); i++) {
		

		row.Init(0, )
		WeightsData.Init()
	}*/
}


void ADriftPawn::WeightsInitRandom() {

}