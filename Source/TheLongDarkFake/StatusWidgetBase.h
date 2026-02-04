// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UStatusWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
    // 블루프린트에 있는 스크롤 박스와 연결 (이름이 일치해야 함)
    UPROPERTY(meta = (BindWidget))
    class UScrollBox* StatusScrollBox;

    // 아이콘으로 사용할 위젯 클래스 (에디터에서 설정)
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> IconWidgetClass;

    virtual void NativeConstruct() override;

    // 델리게이트에 바인딩할 함수
    UFUNCTION()
    void OnStatusUpdate();

    // 실제 UI를 갱신하는 로직
    void RefreshStatusIcons();
	
};
