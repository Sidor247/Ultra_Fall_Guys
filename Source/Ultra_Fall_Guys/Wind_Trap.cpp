// Fill out your copyright notice in the Description page of Project Settings.


#include "Wind_Trap.h"
#include "GameFramework/CharacterMovementComponent.h"

AWind_Trap::AWind_Trap()
{
	PrimaryActorTick.bCanEverTick = true;
	Default_Scene_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Default_Scene_Component"));
	Platform_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform_Mesh"));
	Wind_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation_Box"));
	Wind_Box->InitBoxExtent(FVector(50.0));
}

void AWind_Trap::OnConstruction(const FTransform& Transform)
{
	SetRootComponent(Default_Scene_Root);
	Platform_Mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Wind_Box->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Wind_Box->SetRelativeLocation(Wind_Box_Position);
}

void AWind_Trap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Standing_Character) { 
		FVector velocity = FVector(Wind_Direction, 0.0f) * DeltaSeconds * Push_Force;
		Standing_Character->GetCharacterMovement()->Velocity += velocity;
	}
}

void AWind_Trap::BeginPlay()
{
	Super::BeginPlay();
	Wind_Box->OnComponentBeginOverlap.AddDynamic(this, &AWind_Trap::On_Enter);
	Wind_Box->OnComponentEndOverlap.AddDynamic(this, &AWind_Trap::On_Leave);
	GetWorldTimerManager().SetTimer(
		Timer_Handle,
		[this]() { Wind_Direction = FVector2D(FMath::FRandRange(-1.0f, 1.0f), FMath::FRandRange(-1.0f, 1.0f)).GetSafeNormal(0.0001f); },
		2.0f,
		true
	);
}

void AWind_Trap::EndPlay(const EEndPlayReason::Type Reason)
{
	GetWorldTimerManager().ClearTimer(Timer_Handle);
	Super::EndPlay(Reason);
}

void AWind_Trap::On_Enter(UPrimitiveComponent* Overlapped_Component, AActor* Other_Actor, UPrimitiveComponent* Other_Comp, int32 Other_Body_Index, bool bFrom_Sweep, const FHitResult& Sweep_Result)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(Other_Actor)) {
		Standing_Character = character;
	}
}

void AWind_Trap::On_Leave(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(OtherActor)) {
		if (Standing_Character == character)
			Standing_Character = nullptr;
	}
}

