// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerStat.h"
#include "StatusEffectData.generated.h"

// 1. 개별 효과 파라미터 구조체
USTRUCT(BlueprintType)
struct FDotEffectParams
{
	GENERATED_BODY()
public:
	// 지속적인 피해/회복 (DoT/HoT)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DotEffect")
	float TickInterval = 0.0f; // 효과 적용 주기 (0.0f이면 틱 피해 없음)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DotEffect")
	FName TargetStatID; // 효과가 적용될 스탯 ID (예: Health, Stamina)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DotEffect")
	float DotEffectAmount = 0.0f; // 효과의 최종 양 (피해량, 스탯 변화량 등)
};

USTRUCT(BlueprintType)
struct FMoveMentEffectParams
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MoveMentEffect")
	float MoveEffectAmount = 0.0f; // 효과의 최종 양 (피해량, 스탯 변화량 등)
};

USTRUCT(BlueprintType)
struct FSpecialAbilityEffectParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialAbilityEffect")
	float SpecialEffectAmount = 0.0f; // 효과의 최종 양 (피해량, 스탯 변화량 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialAbilityEffect")
	bool Canlock = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialAbilityEffect")
	bool CanAttack = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialAbilityEffect")
	bool CanJump = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpecialAbilityEffect")
	bool CanSprint = true;
};

// 2. 개별 효과 클래스 참조 및 파라미터 JSON을 담는 Entry 
USTRUCT(BlueprintType)
struct FSingleEffectEntry
{
	GENERATED_BODY()

	// 1. 적용할 효과 클래스 지정 (UDotEffectClass, UMovementDebuffClass 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logic")
	TSubclassOf<UObject> EffectClass;

	// 2. 해당 효과 클래스에 전달할 상세 데이터 (JSON/String 사용으로 유연성 확보)
	// 예시: {"TargetStatID":"Health", "DamagePerTick":-5.0} 
	// 컴포넌트에서 이 String을 파싱하여 DotEffectParams 구조체로 변환합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logic",meta = (Tooltip = "JSON String: 클래스별로 필요한 상세 파라미터를 입력합니다."))
	FString EffectParametersJson;
};

// 3. 마스터 데이터 테이블 구조체
USTRUCT(BlueprintType)
struct FEffectData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 1. 식별 및 표시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName EffectID; // 고유 ID (예: Cold_Exposure, Full_Stomach, Sprained_Ankle)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName; // HUD에 표시될 이름 (예: "추위 노출", "든든함")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	TSoftObjectPtr<UTexture2D> IconTexture; // HUD에 표시될 아이콘

	

	// 2. 효과 속성
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	float DurationSeconds = 0.0f; // 효과의 지속 시간 (0.0f이면 영구 적용)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	bool bIsDebuff = true; // 디버프인지 (Red UI) 버프인지 (Green UI) 구분

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect_Logic")
	TArray<FSingleEffectEntry> EffectEntries;
	

	

	
};

