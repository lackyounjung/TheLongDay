// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestData.generated.h"

USTRUCT(BlueprintType)
struct FObjectiveProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName TargetName;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentCount;

    FObjectiveProgress() : TargetName(NAME_None), CurrentCount(0) {}
    FObjectiveProgress(FName InName, int32 InCount) : TargetName(InName), CurrentCount(InCount) {}
    
};

USTRUCT(BlueprintType)
struct FQuestStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName QuestID; // 데이터 테이블의 Row Name과 일치시킴

   

    UPROPERTY(BlueprintReadOnly)
    TArray<FObjectiveProgress> ObjectiveProgresses;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    FQuestStatus() : QuestID(NAME_None), bIsCompleted(false) {}
    FQuestStatus(FName InID) : QuestID(InID), bIsCompleted(false) {}


};

USTRUCT(BlueprintType)
struct FQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetName; // 목표 대상 (예: "Wolf")

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredCount; // 목표 수치 (예: 5)
};

/**
 * 
 */
 // 퀘스트 목표 유형
UENUM(BlueprintType)
enum class EQuestObjectiveType : uint8
{
    Hunt        UMETA(DisplayName = "Hunt"),
    Gather      UMETA(DisplayName = "Gather"),
    Location    UMETA(DisplayName = "Location"),
    Build       UMETA(DisplayName = "Build"),
    Interact    UMETA(DisplayName = "Interact")
};

// 데이터 테이블용 구조체
USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EQuestObjectiveType ObjectiveType;

    // 대상 식별 (예: Enemy.Fox, Item.Wood 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FName> TargetArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<FQuestObjective> TargetArray2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 TargetCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    float RewardExp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TArray<TSubclassOf<AActor>> RewardItem;
};

//
