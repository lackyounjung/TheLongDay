// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTargetWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UQuestTargetWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    // "늑대 처치" 같은 목표 이름
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TargetNameText;

    // "3 / 5" 같은 진행 수치
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TargetProgressText;

public:
    // 수치를 업데이트하는 함수
    void UpdateTarget(FText TargetName, int32 Current, int32 Max);
	
};
