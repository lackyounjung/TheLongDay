// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWindow.h"
#include "Components/ScrollBox.h"
#include "QuestComponent.h"
#include "QuestSlot.h"

void UQuestWindow::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Warning, TEXT("QuestWindow: NativeConstruct Called"));
    if (APawn* OwningPawn = GetOwningPlayerPawn())
    {
        QuestComp = OwningPawn->FindComponentByClass<UQuestComponent>();
    }
    if (QuestComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestWindow: Component Found and Binding Delegates"));
        QuestComp->OnQuestUpdated.AddDynamic(this, &UQuestWindow::HandleQuestUpdated);
        QuestComp->OnQuestCompleted.AddDynamic(this, &UQuestWindow::HandleQuestCompleted);

        // 처음 창을 열었을 때 한 번 초기화
        RefreshQuestList();
    }
}

void UQuestWindow::HandleQuestUpdated(const FQuestStatus& QuestStatus)
{
    UE_LOG(LogTemp, Warning, TEXT("QuestWindow: HandleQuestUpdated Received!"));
    RefreshQuestList();
    
}

void UQuestWindow::HandleQuestCompleted(FName QuestID)
{
    RefreshQuestList();
}

void UQuestWindow::RefreshQuestList()
{
	if (!QuestComp || !QuestSlotClass) return;

    // 현재 컴포넌트의 실제 개수 로그 찍기
    UE_LOG(LogTemp, Warning, TEXT("Refresh Start - Active: %d, Completed: %d"),
        QuestComp->ActiveQuests.Num(), QuestComp->CompletedQuests.Num());

    // 1. 기존 리스트 초기화
    ActiveQuestScrollBox->ClearChildren();
    CompletedQuestScrollBox->ClearChildren();
    TArray<FQuestStatus> Activeq = QuestComp->ActiveQuests;
    TArray<FName> Completq = QuestComp->CompletedQuests;

    // 2. 진행 중인 퀘스트 목록 순회 (QuestComponent의 데이터를 가져옴)
    for (const FQuestStatus& Status : Activeq)
    {
        UQuestSlot* NewSlot = CreateWidget<UQuestSlot>(this, QuestSlotClass);
        if (NewSlot)
        {
            // 여기서 NewSlot->InitSlot(Status) 같은 함수로 데이터를 전달합니다.
            NewSlot->InitSlot(Status, QuestComp->QuestDataTable);
            ActiveQuestScrollBox->AddChild(NewSlot);
        }
    }
    //3. 완료된 퀘스트 목록
    for (const FName& StatusName : Completq)
    {
        UQuestSlot* NewSlot = CreateWidget<UQuestSlot>(this, QuestSlotClass);
        if (NewSlot)
        {
            // 여기서 NewSlot->InitSlot(Status) 같은 함수로 데이터를 전달합니다.
            NewSlot->InitSlotName(StatusName, QuestComp->QuestDataTable);
            CompletedQuestScrollBox->AddChild(NewSlot);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("QuestWindow :Creating Slot for Quest. Total Active: %d"), Activeq.Num());
}
