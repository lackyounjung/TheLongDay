// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TheLongDarkPlayerController.generated.h"




// 위젯 생성 시 컴포넌트를 직접 주입하기 위해 HUD 위젯 클래스의 전방 선언을 추가합니다.
// (실제 프로젝트에서 사용하는 위젯의 C++ 클래스 이름이 있다면 해당 이름을 사용하세요. 
// 여기서는 일반적인 UUserWidget을 기반으로 진행합니다.)
class UWBP_PlayerHUD;
/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API ATheLongDarkPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	// BeginPlay에서 위젯을 생성하고 뷰포트에 추가하는 로직을 실행합니다.
	virtual void BeginPlay() override;

private:
	// 1. HUD 위젯 블루프린트 에셋을 할당받을 변수
	// TSubclassOf를 사용하면 에디터에서 해당 위젯의 블루프린트 클래스를 선택할 수 있습니다.
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PlayerHUDWidgetClass;

	//// 2. 런타임에 생성된 위젯 인스턴스 (메모리 관리용)
	UPROPERTY()
	UUserWidget* PlayerHUDWidget;
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* GetPlayerHUDWidget() const { return PlayerHUDWidget; }
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnHUDCreated(UUserWidget* CreatedHUD);
	
};
