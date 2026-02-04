// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h" 
#include "Particles/ParticleSystem.h" 
#include "WeatherSystemData.generated.h"

// 날씨의 유형을 정의
UENUM(BlueprintType)
enum class EWeatherCondition : uint8
{
	EWC_Clear       UMETA(DisplayName = "Clear"),      // 맑음
	EWC_Snow        UMETA(DisplayName = "Snow"),       // 눈
	EWC_Blizzard    UMETA(DisplayName = "Blizzard"),   // 눈보라 (강한 디버프 유발)
	EWC_Fog         UMETA(DisplayName = "Fog"),        // 안개
};

USTRUCT(BlueprintType)
struct FWeatherData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 1. 식별 및 시각 효과
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName WeatherID; // 고유 ID (예: Default_Snow, High_Wind_Blizzard)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	EWeatherCondition Condition = EWeatherCondition::EWC_Clear; // 날씨 유형

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
	TSoftObjectPtr<UParticleSystem> ParticleEffect; // 눈, 안개 등 시각 효과

	// 2. 환경 영향 (생존 스탯 보정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Survival_Impact")
	float TemperatureOffset = 0.0f; // 환경 기본 온도에 더하거나 뺄 값 (예: 눈보라 시 -15도)

	// Cold 스탯 감소율에 대한 보정값 (예: 1.5f면 Cold 감소율 50% 증가)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Survival_Impact")
	float ColdDecreaseRateMultiplier = 1.0f;

	// 이동 속도에 대한 보정값 (눈보라 시 이동 속도 20% 감소 -> 0.8f)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Survival_Impact")
	float MovementSpeedMultiplier = 1.0f;

	// 3. 기타 환경 영향
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Other_Impact")
	float DetectionRangeMultiplier = 1.0f; // 안개 시 동물의 감지 범위 감소 등 (예: 0.5f)
};
