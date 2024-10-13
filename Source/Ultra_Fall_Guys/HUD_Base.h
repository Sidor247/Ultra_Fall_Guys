// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD_Base.generated.h"

/**
 * 
 */
UCLASS()
class ULTRA_FALL_GUYS_API UHUD_Base : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly) FString Health_Text;
	UPROPERTY(BlueprintReadOnly) FString Time_Text;

	void Set_Health(int New_Health);
	void Set_Time(float Seconds);
};
