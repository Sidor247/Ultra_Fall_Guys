#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Finish_Trigger.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API AFinish_Trigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AFinish_Trigger();

private:
	UFUNCTION() void On_Enter(UPrimitiveComponent* Overlapped_Component, AActor* Other_Actor, UPrimitiveComponent* Other_Comp, int32 Other_Body_Index, bool bFrom_Sweep, const FHitResult& Sweep_Result);
};
