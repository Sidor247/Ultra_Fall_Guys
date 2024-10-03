// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ultra_Fall_GuysGameMode.h"
#include "Ultra_Fall_GuysCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUltra_Fall_GuysGameMode::AUltra_Fall_GuysGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
