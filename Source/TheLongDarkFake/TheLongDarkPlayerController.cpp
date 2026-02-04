// Fill out your copyright notice in the Description page of Project Settings.


#include "TheLongDarkPlayerController.h"
#include "Blueprint/UserWidget.h" // UMG 위젯 생성 함수를 사용하기 위해 필요
#include "CPP_PlayerHUD.h"
#include "Framework/Application/SlateApplication.h"
#include "PlayerSurvivalStatComponent.h"

void ATheLongDarkPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 1. 마우스 커서를 숨깁니다.
	bShowMouseCursor = false;

	// 2. 게임 전용 입력 모드를 설정합니다.
	FInputModeGameOnly InputMode;

	InputMode.SetConsumeCaptureMouseDown(true);




	// 5. 컨트롤러에 입력 모드를 적용합니다.
	SetInputMode(InputMode);

	bShowMouseCursor = false;

	// Player Pawn 가져오기 (이미 BeginPlay 시점에 Pawn이 소유되어 있을 가능성이 높음)
	APawn* PlayerPawn = GetPawn();

	
	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<UUserWidget>(this, PlayerHUDWidgetClass);
		PlayerHUDWidget->AddToViewport();
		PlayerHUDWidget->SetVisibility(ESlateVisibility::Hidden);
		OnHUDCreated(PlayerHUDWidget);
	}
	
	
}
