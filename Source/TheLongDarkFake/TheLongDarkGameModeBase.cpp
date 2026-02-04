// Fill out your copyright notice in the Description page of Project Settings.


#include "TheLongDarkGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ATheLongDarkGameModeBase::ATheLongDarkGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_Player.BP_Player_C"));
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerController(TEXT("/Game/Player/BP_TheLongDarkPlayerController.BP_TheLongDarkPlayerController_C"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	if (PlayerController.Succeeded())
	{
		PlayerControllerClass = PlayerController.Class;
	}
}
