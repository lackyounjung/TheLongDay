// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "QuestTargetWidget.h"
#include "QuestComponent.h"

void UQuestSlot::InitSlot(const FQuestStatus& Status, UDataTable* QuestDataTable)
{
    if (!QuestDataTable) return;

    // 1. 데이터 테이블에서 퀘스트 상세 정보 찾기
    static const FString ContextString(TEXT("Quest Slot Context"));
    FQuestData* Data = QuestDataTable->FindRow<FQuestData>(Status.QuestID, ContextString);

    if (Data)
    {
        // 2. 퀘스트 이름 설정
        QuestNameText->SetText(Data->QuestName);

        // 3. 기존 타겟 리스트 초기화
        TargetListContext->ClearChildren();

        // 4. 데이터에 들어있는 타겟 수만큼 위젯 생성
        // 예: 늑대 5마리 잡기, 약초 3개 캐기 등
        for (int32 i = 0; i < Data->TargetArray2.Num(); ++i)
        {
            if (TargetWidgetClass)
            {
                UQuestTargetWidget* TargetWidget = CreateWidget<UQuestTargetWidget>(this, TargetWidgetClass);
                if (TargetWidget)
                {
                    // 타겟 위젯에 데이터 전달 (타겟 이름, 현재 수치, 목표 수치)
                    // Status.TargetCounts[i]는 현재 진행도를 의미한다고 가정
                    TargetWidget->UpdateTarget(FText::FromName(Data->TargetArray2[i].TargetName),
                        Status.ObjectiveProgresses[i].CurrentCount,
                        Data->TargetArray2[i].RequiredCount
                    );

                    TargetListContext->AddChild(TargetWidget);
                }
            }
        }

        QuestDescription->SetText(Data->Description);

    }
}

void UQuestSlot::InitSlotName(FName QuestName, class UDataTable* QuestDataTable)
{
    static const FString ContextString(TEXT("Quest Slot Context"));
    FQuestData* Data = QuestDataTable->FindRow<FQuestData>(QuestName, ContextString);
    if (Data)
    {
        // 2. 퀘스트 이름 설정
        QuestNameText->SetText(Data->QuestName);
        QuestNameText->SetColorAndOpacity(FLinearColor::Green);

        // 3. 기존 타겟 리스트 초기화
        TargetListContext->ClearChildren();

        

        QuestDescription->SetText(FText::FromString(TEXT("")));
        
        
    }
}
