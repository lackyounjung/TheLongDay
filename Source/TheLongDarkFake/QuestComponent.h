// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MySaveGame.h"
#include "QuestData.h"
#include "QuestComponent.generated.h"



// 퀘스트 업데이트 알림을 위한 델리게이트 (UI 연동용)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, const FQuestStatus&, QuestStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, FName, QuestID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UQuestComponent();

    // 1. 퀘스트 수락
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void AcceptQuest(FName QuestID);

    // 2. 외부 액션 알림 (사냥, 채집, 건설 등 발생 시 호출)
    // 예: NotifyAction(EQuestObjectiveType::Hunt, "Fox");
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void NotifyAction(EQuestObjectiveType ObjectiveType, FName TargetName);

    void CheckQuestCompletion(FQuestStatus& Status, FQuestData* Data);

    // 3. 퀘스트 완료 처리 (보상 지급 등)
    void CompleteQuest(FName QuestID);

    // 4. 현재 진행 중인 퀘스트 상태 찾기 (편의용)
    FQuestStatus* GetQuestStatus(FName QuestID);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    // 퀘스트 원본 데이터 테이블 (에디터에서 할당)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
    UDataTable* QuestDataTable;

    // 현재 진행 중인 퀘스트 목록
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
    TArray<FQuestStatus> ActiveQuests;

    // 완료된 퀘스트 ID 목록 (중복 수락 방지 및 저장용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
    TArray<FName> CompletedQuests;

    // UI에서 바인딩할 이벤트
    UPROPERTY(BlueprintAssignable, Category = "Quest")
    FOnQuestUpdated OnQuestUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Quest")
    FOnQuestCompleted OnQuestCompleted;

    void GetQuestDataForSave(TArray<FQuestProgressSaveData>& OutActive, TArray<FName>& OutCompleted);
    void LoadQuestData(const TArray<FQuestProgressSaveData>& InActive, const TArray<FName>& InCompleted);

		
};
