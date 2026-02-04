// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimeWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UTimeWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	// 타임 매니저로부터 Float 시간(0.0~24.0)을 받아 UI를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI|Time")
	void UpdateTime(float InCurrentTime);

protected:
	// 시간 숫자를 표시할 텍스트 (WBP에서 IsVariable 체크 필수)
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	// 오전/오후 표시가 필요할 경우 (선택 사항)
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AMPMText;

private:
	// 매 프레임 텍스트를 갱신하지 않기 위한 캐싱 변수
	int32 LastDisplayedMinute = -1;
	
};
