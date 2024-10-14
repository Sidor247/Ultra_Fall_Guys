#include "Total_Screen_Base.h"
#include "Kismet/KismetMathLibrary.h"

void UTotal_Screen_Base::Set_Result(bool bIs_Win, float time)
{
	if (bIs_Win) {
		Game_Result_Text = TEXT("VICTORY!");
		FTimespan timespan = UKismetMathLibrary::FromSeconds(time);
		Time_Text = FString::Printf(
			TEXT("Your time: %d:%02d:%02d.%03d"),
			timespan.GetHours(),
			timespan.GetMinutes(),
			timespan.GetSeconds(),
			timespan.GetFractionMilli()
		);
	} else {
		Game_Result_Text = TEXT("DEFEAT!");
		Time_Text = TEXT("");
	}
}
