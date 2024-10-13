// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerTrap.h"
#include "Ultra_Fall_GuysCharacter.h"

ATimerTrap::ATimerTrap()
{
	PrimaryActorTick.bCanEverTick = true;
	Default_Scene_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Default_Scene_Component"));
	SM_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static_Mesh"));
	Activation_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Activation_Box"));
}

void ATimerTrap::BeginPlay()
{
	Super::BeginPlay();
	Activation_Box->OnComponentBeginOverlap.AddDynamic(this, &ATimerTrap::On_Platform_Enter);
	Activation_Box->OnComponentEndOverlap.AddDynamic(this, &ATimerTrap::On_Platform_Leave);
}

void ATimerTrap::OnConstruction(const FTransform& Transform)
{
	SetRootComponent(Default_Scene_Root);
	SM_Component->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Activation_Box->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	Handle_New_State(State::Ready);
	Activation_Box->SetBoxExtent(FVector(50.0, 50.0, 10.0), false);
	Activation_Box->SetRelativeLocation(Activation_Box_Position);
}

void ATimerTrap::Tick(float DeltaSeconds)
{
	if (Current_State == Damaging && Standing_Character && !Already_Damaged) {
		Standing_Character->Handle_Damage(Damage);
		Already_Damaged = true;
	}
}

void ATimerTrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
 	GetWorldTimerManager().ClearTimer(Timer_Handle);
}

void ATimerTrap::On_Platform_Enter(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(OtherActor)) {
		Standing_Character = character;
		if (Current_State == Ready)
			Handle_New_State(Preparing);
	}	
}

void ATimerTrap::On_Platform_Leave(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32)
{
	if (auto character = Cast<AUltra_Fall_GuysCharacter>(OtherActor)) {
		if (character == Standing_Character)
			Standing_Character = nullptr;
	}
}

void ATimerTrap::Handle_New_State(State New_State)
{
	switch (New_State) {
	case Ready:
		Already_Damaged = false;
		if (SM_Component)
			SM_Component->SetMaterial(0, Idle_Material);
		if (Standing_Character)
			Handle_New_State(Preparing);
		break;

	case Preparing:
		if (SM_Component)
			SM_Component->SetMaterial(0, Preparing_Material);
		GetWorld()->GetTimerManager().SetTimer(
			Timer_Handle,
			[this]() { Handle_New_State(Damaging); },
			Preparing_Time,
			false
		);
		break;

	case Damaging:
		if (SM_Component)
			SM_Component->SetMaterial(0, Damaging_Material);
		GetWorld()->GetTimerManager().SetTimer(
			Timer_Handle,
			[this]() { Handle_New_State(Reloading); },
			Damaging_Time,
			false
		);
		break;

	case Reloading:
		if (SM_Component)
			SM_Component->SetMaterial(0, Reloading_Material);
		GetWorld()->GetTimerManager().SetTimer(
			Timer_Handle,
			[this]() { Handle_New_State(Ready); },
			Reloading_Time,
			false
		);
		break;
	}
	Current_State = New_State;
}

