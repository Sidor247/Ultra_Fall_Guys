#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformGenerator.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API APlatformGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	APlatformGenerator();

	UPROPERTY(EditAnywhere, Category = "Generation Settings") UStaticMeshComponent* SM_Root_Component;
	UPROPERTY(EditAnywhere, Category = "Generation Settings") int Count = 1;
	UPROPERTY(EditAnywhere, Category = "Generation Settings") double Padding = 100.0;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	TArray<UStaticMeshComponent*> SM_Array;
};
