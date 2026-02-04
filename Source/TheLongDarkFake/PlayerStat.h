// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "PlayerStat.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerStat : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FName StatID; //스탯을 식별하는 고유 ID. (예: Hunger, Thirst, Cold, Health)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FText Name; //UI에 표시될 스탯의 이름 (다국어 지원 고려).
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxValue; //스탯의 최대값. (일반적으로 100.0)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float InitialValue; //게임 시작 또는 리스폰 시 적용될 초기값.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float DecreaseRatePerSec; //1초당 감소하는 스탯 값. (예: 배고픔 0.05/sec)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float CriticalThreshold; //스탯이 이 값 이하로 떨어지면 위험 상태로 간주하는 임계치.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FName DebuffEffectID; //위험 임계치 도달 시 적용할 디버프 효과의 ID (예: Slow_Movement, Shivering).

};
