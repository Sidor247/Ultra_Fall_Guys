// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ultra_Fall_GuysCharacter.h"
#include <Components/BoxComponent.h>
#include "TimerTrap.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API ATimerTrap : public AActor
{
	GENERATED_BODY()
	
public:
	ATimerTrap();

	UPROPERTY(EditAnywhere) UStaticMeshComponent* SM_Component;
	UPROPERTY(EditAnywhere) UBoxComponent* Activation_Box;

	UPROPERTY(EditAnywhere) UMaterialInterface* Idle_Material;
	UPROPERTY(EditAnywhere) UMaterialInterface* Preparing_Material;
	UPROPERTY(EditAnywhere) UMaterialInterface* Damaging_Material;
	UPROPERTY(EditAnywhere) UMaterialInterface* Reloading_Material;

	UPROPERTY(EditAnywhere) double Preparing_Time = 1.0;
	UPROPERTY(EditAnywhere) double Damaging_Time = 1.0;
	UPROPERTY(EditAnywhere) double Reloading_Time = 1.0;

	UPROPERTY(EditAnywhere) FVector Activation_Box_Position = FVector::ZeroVector;

	UPROPERTY(EditAnywhere) USceneComponent* Default_Scene_Root;

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION() void On_Platform_Enter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bSweepResult, const FHitResult& HitResult);
	UFUNCTION() void On_Platform_Leave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex);


	enum State { Ready, Preparing, Damaging, Reloading } Current_State;
	void Handle_New_State(State New_State);

	AUltra_Fall_GuysCharacter* Standing_Character = nullptr;
	bool Already_Damaged = false;
};
