// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestData.h"
#include "QuestSlot.generated.h"


/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UQuestSlot : public UUserWidget
{
	GENERATED_BODY()
protected:
    // WBP_QuestSlot에 있는 텍스트 블록
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestNameText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestDescription;

    // WBP_QuestSlot에 있는 수직 박스 (타겟 슬롯들이 들어갈 곳)
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* TargetListContext;

    // 타겟 하나하나를 표시할 위젯 클래스 (WBP_QuestTargetSlot)
    UPROPERTY(EditAnywhere, Category = "Quest UI")
    TSubclassOf<UUserWidget> TargetWidgetClass;

public:
    // 데이터를 받아 위젯을 채우는 함수
    void InitSlot(const FQuestStatus& Status, class UDataTable* QuestDataTable);
    void InitSlotName(FName QuestName, class UDataTable* QuestDataTable);
	
};
