// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "CraftingBase.h"
#include "ItemBase.h"
#include "BuildItemBase.h"
#include "InventoryComponent.h"
#include "CraftingComponent.h"
#include "BuildComponent.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UQuestComponent::AcceptQuest(FName QuestID)
{
	// 이미 완료했거나 진행 중인지 체크
	if (CompletedQuests.Contains(QuestID) || GetQuestStatus(QuestID)) return;

	if (QuestDataTable)
	{
		FQuestData* Data = QuestDataTable->FindRow<FQuestData>(QuestID, TEXT("QuestComponent_AcceptProgress"));
		if (Data)
		{
			FQuestStatus NewStatus(QuestID);
			for (const FQuestObjective& Objective : Data->TargetArray2)
			{
				// 데이터 테이블에 정의된 모든 목표를 '0' 상태로 추가
				NewStatus.ObjectiveProgresses.Add(FObjectiveProgress(Objective.TargetName, 0));
			}
			ActiveQuests.Add(NewStatus);
			OnQuestUpdated.Broadcast(NewStatus);
			UE_LOG(LogTemp, Warning, TEXT("Quest Accepted: %s"), *QuestID.ToString());
		}
	}
}

void UQuestComponent::NotifyAction(EQuestObjectiveType ObjectiveType, FName TargetName)
{
	UE_LOG(LogTemp, Warning, TEXT("=== NotifyAction Debug Start ==="));
	UE_LOG(LogTemp, Warning, TEXT("Input Target: [%s], Input Type: [%d]"), *TargetName.ToString(), (int32)ObjectiveType);
	// 현재 진행 중인 모든 퀘스트를 검사
	for (int32 i = ActiveQuests.Num() - 1; i >= 0; --i)
	{
		FQuestStatus& Status = ActiveQuests[i];
		FQuestData* Data = QuestDataTable->FindRow<FQuestData>(Status.QuestID, TEXT("QuestComponent_UpdateProgress"));

		if (!Data) {
			UE_LOG(LogTemp, Error, TEXT("Data Null for Quest: %s"), *Status.QuestID.ToString());
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("Quest [%s] - Data Type: [%d] vs Input Type: [%d]"),
			*Status.QuestID.ToString(), (int32)Data->ObjectiveType, (int32)ObjectiveType);

		if (Data && Data->ObjectiveType == ObjectiveType)
		{
			bool bProgressMade = false;
			for (int32 ObjIdx = 0; ObjIdx < Data->TargetArray2.Num(); ++ObjIdx)
			{

				FName RequiredName = Data->TargetArray2[ObjIdx].TargetName;
				UE_LOG(LogTemp, Log, TEXT("Index [%d] - Checking Required Name: [%s] against Input: [%s]"),
					ObjIdx, *RequiredName.ToString(), *TargetName.ToString());

				if (Data->TargetArray2[ObjIdx].TargetName == TargetName)
				{
					// 4. Status의 대응하는 인덱스 카운트 증가
					// (주의: ObjectiveProgresses 배열 크기가 TargetArray2와 동일하게 초기화되어 있어야 함)
					if (Status.ObjectiveProgresses.IsValidIndex(ObjIdx))
					{
						// 이미 목표치를 달성했다면 더 이상 올리지 않음 (선택 사항)
						if (Status.ObjectiveProgresses[ObjIdx].CurrentCount < Data->TargetArray2[ObjIdx].RequiredCount)
						{
							Status.ObjectiveProgresses[ObjIdx].CurrentCount++;
							UE_LOG(LogTemp, Warning, TEXT("Quest notify: %s"), *TargetName.ToString());
							bProgressMade = true;
						}
					}
				}
			}
			// 5. 변화가 있었다면 UI에 알리고 완료 여부 체크
			if (bProgressMade)
			{
				OnQuestUpdated.Broadcast(Status);
				

				// 모든 세부 목표를 달성했는지 체크하는 함수 호출
				CheckQuestCompletion(Status, Data);
			}
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Type Mismatch! Quest Type is different from NotifyAction Type."));
		}

	}
	UE_LOG(LogTemp, Warning, TEXT("=== NotifyAction Debug End ==="));
}

void UQuestComponent::CheckQuestCompletion(FQuestStatus& Status, FQuestData* Data)
{
	if (!Data) return;

	bool bAllFinished = true;

	for (int32 i = 0; i < Data->TargetArray2.Num(); ++i)
	{
		// 하나라도 목표치에 도달하지 못했다면 완료 아님
		if (Status.ObjectiveProgresses[i].CurrentCount < Data->TargetArray2[i].RequiredCount)
		{
			bAllFinished = false;
			break;
		}
	}

	if (bAllFinished)
	{
		CompleteQuest(Status.QuestID);
	}
}

void UQuestComponent::CompleteQuest(FName QuestID)
{
	FQuestStatus* Status = GetQuestStatus(QuestID);
	if (Status)
	{
		Status->bIsCompleted = true;
		CompletedQuests.Add(QuestID);

		// 보상 로직 (예: 경험치 추가)
		FQuestData* Data = QuestDataTable->FindRow<FQuestData>(QuestID, "");
		if (Data)
		{ 
			/* Player->AddExp(Data->RewardExp); */
			int rewardnum = Data->RewardItem.Num();
			for (int i = 0; i < rewardnum; i++)
			{
				UClass* RewardClass = Data->RewardItem[i];
				if (RewardClass->IsChildOf(AItemBase::StaticClass()))
				{
					UInventoryComponent* InvComp = GetOwner()->FindComponentByClass<UInventoryComponent>();
					AItemBase* itemBasePtr = RewardClass->GetDefaultObject<AItemBase>();
					if (InvComp)
					{
						InvComp->AddItem(itemBasePtr->GetItemID());
					}

						
				}
				else if (RewardClass->IsChildOf(ACraftingBase::StaticClass()))
				{
					UCraftingComponent* CraftComp = GetOwner()->FindComponentByClass<UCraftingComponent>();
					ACraftingBase* CraftBasePtr = RewardClass->GetDefaultObject<ACraftingBase>();
					if (CraftComp)
					{
						CraftComp->FindCraftingRecipe(CraftBasePtr->GetRecipeName());
					}
				}
				else if (RewardClass->IsChildOf(ABuildItemBase::StaticClass()))
				{
					UInventoryComponent* InvComp = GetOwner()->FindComponentByClass<UInventoryComponent>();
					ABuildItemBase* BuildBasePtr = RewardClass->GetDefaultObject<ABuildItemBase>();
					if (InvComp)
					{
						InvComp->AddItem(BuildBasePtr->GetItemID());
					}
				}
			}
		}

		// 진행 목록에서 삭제 (또는 상태 유지)
		ActiveQuests.RemoveAll([QuestID](const FQuestStatus& S) { return S.QuestID == QuestID; });

		OnQuestUpdated.Broadcast(*Status);
		OnQuestCompleted.Broadcast(QuestID);

		

		UE_LOG(LogTemp, Warning, TEXT("Quest Completed: %s"), *QuestID.ToString());
	}
}

FQuestStatus* UQuestComponent::GetQuestStatus(FName QuestID)
{
	return ActiveQuests.FindByPredicate([&](const FQuestStatus& Status) {
		return Status.QuestID == QuestID;
		});
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::GetQuestDataForSave(TArray<FQuestProgressSaveData>& OutActive, TArray<FName>& OutCompleted)
{
	OutActive.Empty();
	for (const FQuestStatus& Status : ActiveQuests)
	{
		FQuestProgressSaveData Data;
		Data.QuestID = Status.QuestID;
		for (const FObjectiveProgress& ObjProgress : Status.ObjectiveProgresses)
		{
			Data.ObjectiveCounts.Add(ObjProgress.CurrentCount);
		}
		OutActive.Add(Data);
	}
	OutCompleted = CompletedQuests;
}

void UQuestComponent::LoadQuestData(const TArray<FQuestProgressSaveData>& InActive, const TArray<FName>& InCompleted)
{
	CompletedQuests = InCompleted;
	ActiveQuests.Empty();

	for (const FQuestProgressSaveData& Data : InActive)
	{
		// 1. 퀘스트를 새로 수락하는 로직(AcceptQuest)과 유사하지만, 데이터를 강제 주입
		if (QuestDataTable)
		{
			FQuestData* QuestRow = QuestDataTable->FindRow<FQuestData>(Data.QuestID, TEXT("QuestLoad"));
			if (QuestRow)
			{
				FQuestStatus LoadedStatus(Data.QuestID);
				for (int32 i = 0; i < QuestRow->TargetArray2.Num(); ++i)
				{
					int32 SavedCount = Data.ObjectiveCounts.IsValidIndex(i) ? Data.ObjectiveCounts[i] : 0;
					LoadedStatus.ObjectiveProgresses.Add(FObjectiveProgress(QuestRow->TargetArray2[i].TargetName, SavedCount));
				}
				ActiveQuests.Add(LoadedStatus);
				// UI 갱신을 위해 브로드캐스트
				OnQuestUpdated.Broadcast(LoadedStatus);
			}
		}
	}
}

