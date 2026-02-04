// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ResourceData.generated.h"

// 채집 자원의 유형을 정의합니다.
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	ERT_Tree        UMETA(DisplayName = "Tree"),          // 나무/장작
	ERT_Stone       UMETA(DisplayName = "Stone"),         // 돌/광물
	ERT_Bush        UMETA(DisplayName = "Bush"),          // 식물/덤불/약초
	ERT_Corpse      UMETA(DisplayName = "Corpse"),        // 동물 사체 (도축/가죽 벗기기)
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct FResourceData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 1. 식별 및 유형
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName ResourceID; // 고유 ID (예: PineTree_Large, Flint_Deposit)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EResourceType ResourceType = EResourceType::ERT_Tree; // 자원의 유형

	// 2. 획득 정보 (드롭 아이템)
	// 이전 CreatureData와 마찬가지로, 아이템 ID와 수량을 묶어 확률을 도입하는 것이 좋습니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting")
	TArray<FName> DropItemID; // 채집 성공 시 획득할 아이템의 Master ID (예: Wood_Scrap, Stone_Small)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting")
	TArray<int32> MinQuantity; // 각 아이템의 최소 획득 수량

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Harvesting")
	TArray<int32> MaxQuantity; // 각 아이템의 최대 획득 수량 (랜덤 범위)

	// 3. 월드 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	float MaxHarvestAmount = 10.0f; // 이 자원 노드에서 총 채집 가능한 양 (예: 나무를 10번 찍을 수 있음)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	float RespawnTimeSeconds = 0.0f; // 자원이 재생성되는 데 걸리는 시간 (0이면 재생성 안됨)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TSoftObjectPtr<UStaticMesh> MeshPath; // 월드에 배치될 이 자원의 3D 모델 경로
};
