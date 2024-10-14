#include "Bouncing_Trap.h"
#include "Ultra_Fall_GuysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ABouncing_Trap::ABouncing_Trap()
{
	PrimaryActorTick.bCanEverTick = true;
	SM_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static_Mesh"));
	RootComponent = SM_Component;
}

void ABouncing_Trap::BeginPlay()
{
	Super::BeginPlay();
	OnActorHit.AddDynamic(this, &ABouncing_Trap::On_Hit);
}

void ABouncing_Trap::On_Hit(AActor*, AActor* Other_Actor, FVector, const FHitResult& Hit)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(Other_Actor)) {
		character->GetCharacterMovement()->Velocity = -Bounce_Force * Hit.Normal;
	}
}

