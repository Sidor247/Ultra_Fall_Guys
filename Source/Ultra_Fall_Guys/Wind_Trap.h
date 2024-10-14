#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Ultra_Fall_GuysCharacter.h"
#include "Wind_Trap.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API AWind_Trap : public AActor
{
	GENERATED_BODY()
	
public:	
	AWind_Trap();

	UPROPERTY(EditAnywhere) float Push_Force = 1000.0f;
	UPROPERTY(EditAnywhere) FVector Wind_Box_Position = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere) UStaticMeshComponent* Platform_Mesh;
	UPROPERTY(EditAnywhere) USceneComponent* Default_Scene_Root;
	UPROPERTY(EditAnywhere) UBoxComponent* Wind_Box;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	UFUNCTION() void On_Enter(UPrimitiveComponent* Overlapped_Component, AActor* Other_Actor, UPrimitiveComponent* Other_Comp, int32 Other_Body_Index, bool bFrom_Sweep, const FHitResult& Sweep_Result);
	UFUNCTION() void On_Leave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex);

	AUltra_Fall_GuysCharacter* Standing_Character = nullptr;
	FTimerHandle Timer_Handle;
	FVector2D Wind_Direction;
};
