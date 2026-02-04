// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractData.generated.h"

// 상호작용 오브젝트의 유형을 정의합니다.
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	EIT_Door        UMETA(DisplayName = "Door"),          // 열고 닫는 문
	EIT_Container   UMETA(DisplayName = "Container"),     // 보관함/서랍 (아이템 루팅)
	EIT_Bed         UMETA(DisplayName = "Bed"),           // 휴식/취침 가능 오브젝트
	EIT_Terminal    UMETA(DisplayName = "Terminal"),      // 스위치/터미널 등 기능성 오브젝트
};

USTRUCT(BlueprintType)
struct FInteractData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 1. 식별 및 유형
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName ObjectID; // 고유 ID (예: HouseDoor_Cabin01, Locker_Medical)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EInteractableType ObjectType = EInteractableType::EIT_Door; // 오브젝트의 유형

	// 2. 상호작용 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText InteractionText; // 화면에 표시될 상호작용 메시지 (예: "열기", "잠자기", "수색하기")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bIsLocked = false; // 기본적으로 잠겨있는지 여부

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FName RequiredKeyItemID; // 잠겨있을 경우 필요한 키 아이템의 Master ID ("None"이면 키 불필요)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bRequiresCondition = false; // 상호작용에 추가 조건이 필요한지 (예: 도끼가 있어야 문을 부숨)

	// 3. 루팅/기타
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	FName LootTableID; // Container 유형일 경우, 이 보관함이 참조할 별도의 루팅 테이블 ID
};
/**
 * 
 */

