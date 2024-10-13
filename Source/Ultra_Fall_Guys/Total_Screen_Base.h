#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Total_Screen_Base.generated.h"

UCLASS()
class ULTRA_FALL_GUYS_API UTotal_Screen_Base : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Set_Result(bool bIs_Win, float time = 0.0f);

	UPROPERTY(BlueprintReadOnly) FString Game_Result_Text;
	UPROPERTY(BlueprintReadOnly) FString Game_Result_Color;
	UPROPERTY(BlueprintReadOnly) FString Time_Text;
};
