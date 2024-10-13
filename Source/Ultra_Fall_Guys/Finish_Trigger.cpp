#include "Finish_Trigger.h"
#include <Components/BoxComponent.h>
#include "Ultra_Fall_GuysCharacter.h"

AFinish_Trigger::AFinish_Trigger()
{
	PrimaryActorTick.bCanEverTick = false;
	UBoxComponent* box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	box->InitBoxExtent(FVector(50.0));
	RootComponent = box;
	box->OnComponentBeginOverlap.AddDynamic(this, &AFinish_Trigger::On_Enter);
}

void AFinish_Trigger::On_Enter(UPrimitiveComponent* Overlapped_Component, AActor* Other_Actor, UPrimitiveComponent* Other_Comp, int32 Other_Body_Index, bool bFrom_Sweep, const FHitResult& Sweep_Result)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(Other_Actor)) {
		character->Handle_Finish();
	}
}


