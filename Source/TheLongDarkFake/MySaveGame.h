// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Item_Data.h"
#include "CraftingData.h"
#include "MySaveGame.generated.h"

// MySaveGame.h 에 추가

USTRUCT(BlueprintType)
struct FQuestProgressSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName QuestID;

    // 각 세부 목표의 현재 카운트들 (FObjectiveProgress의 CurrentCount들)
    UPROPERTY()
    TArray<int32> ObjectiveCounts;
};

USTRUCT(BlueprintType)
struct FSavedEffectData
{
    GENERATED_BODY()

    UPROPERTY()
    FName EffectID;

    UPROPERTY()
    float RemainingTime;
};

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
    UMySaveGame();

    // 플레이어 변수
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FVector PlayerLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FRotator PlayerRotation;

    // 인벤토리 컴포넌트 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FItem_Data> SavedInventoryItems;

    // 크래프팅 컴포넌트 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FCraftingRecipe> SavedRecipes;

    // 슬롯 관리용
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FString SaveSlotName;

    // 장착 데이터: 소켓 이름(Key)과 아이템 ID(Value)
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TMap<FString, FName> SavedEquippedItems;

    // 월드 시간 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float SavedWorldTime;

    // 스탯 데이터 (StatID : CurrentValue)
    UPROPERTY()
    TMap<FName, float> SavedStats;

    // 활성화된 효과 리스트
    UPROPERTY()
    TArray<FSavedEffectData> SavedActiveEffects;

    UPROPERTY()
    TArray<FQuestProgressSaveData> SavedActiveQuests;

    UPROPERTY()
    TArray<FName> SavedCompletedQuests;
};
