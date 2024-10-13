// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD_Base.h"
#include "Kismet\KismetMathLibrary.h"

void UHUD_Base::Set_Health(int New_Health)
{
	Health_Text = FString::Printf(TEXT("Health: %d"), New_Health);
}

void UHUD_Base::Set_Time(float Seconds)
{
	FTimespan timespan = UKismetMathLibrary::FromSeconds(Seconds);
	Time_Text = FString::Printf(
		TEXT("Time: %d:%02d:%02d.%03d"),
		timespan.GetHours(),
		timespan.GetMinutes(),
		timespan.GetSeconds(),
		timespan.GetFractionMilli()
	);
}
