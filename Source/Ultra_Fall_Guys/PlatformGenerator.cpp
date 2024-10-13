#include "PlatformGenerator.h"


APlatformGenerator::APlatformGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	SM_Root_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(SM_Root_Component);
}

void APlatformGenerator::OnConstruction(const FTransform& Transform)
{
	for (auto It = SM_Array.CreateIterator(); It; It++)
		(*It)->DestroyComponent();
	SM_Array.Empty();

	RegisterAllComponents();

	for (int i = 0; i < Count; ++i) {
		FName component_name(*FString::Printf(TEXT("Platform %d"), i + 1));
		UStaticMeshComponent* new_component = NewObject<UStaticMeshComponent>(this, component_name);
		if (new_component) {
			SM_Array.Add(new_component);
			new_component->RegisterComponent();
			new_component->SetStaticMesh(SM_Root_Component->GetStaticMesh());
			new_component->SetMaterial(0, SM_Root_Component->GetMaterial(0));
			new_component->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			FVector location = FVector::ZeroVector;
			location.X = i * Padding;
			new_component->SetRelativeLocation(location);
		}
	}
}
