#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bouncing_Trap.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API ABouncing_Trap : public AActor
{
	GENERATED_BODY()
	
public:	
	ABouncing_Trap();

	UPROPERTY(EditAnywhere) UStaticMeshComponent* SM_Component;
	UPROPERTY(EditAnywhere) float Bounce_Force = 1000.0f;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION() void On_Hit(AActor* Self_Actor, AActor* Other_Actor, FVector Normal_Impulse, const FHitResult& Hit);
};
