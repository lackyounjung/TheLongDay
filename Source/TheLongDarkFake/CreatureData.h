// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CreatureData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EBehaviorType : uint8
{
	EBT_Passive    UMETA(DisplayName = "Passive"),    // 토끼, 사슴: 먼저 공격하지 않고 도망감
	EBT_Aggressive UMETA(DisplayName = "Aggressive"),  // 늑대, 곰: 플레이어를 보면 먼저 공격
	EBT_Neutral    UMETA(DisplayName = "Neutral"),    // 곰: 가까이 가면 공격
	EBT_Static     UMETA(DisplayName = "Static"),     // 물고기, 벌레 (자원으로 취급)
};

USTRUCT(BlueprintType)
struct FCreatureData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	FName CreatureID; //동물을 식별하는 고유 ID. (예: Wolf_Grey, Deer_Male, Rabbit)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	FText Name; //UI 또는 로그에 표시될 동물의 이름.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	float MaxHealth; //동물의 최대 체력. 0이 되면 사망/사냥 성공으로 간주.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	EBehaviorType BehaviorType; //동물의 기본 행동 패턴. (예: Passive, Aggressive, Fleeing)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	float DetectionRange; //플레이어를 감지하는 시야/청각 범위 (유닛).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	TArray<FName> DropItemID; // 사냥 성공 시 확정적으로 드롭되는 아이템 ID 목록.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	TArray<int32> DropQuantity; //DropItemID와 1:1로 매칭되는 각 아이템의 수량. (예: 고기, 가죽)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Creature")
	float CombatDamage; //공격적인 동물일 경우 플레이어에게 입히는 기본 피해량.
};
