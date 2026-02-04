// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SetEfectInterface.h"
#include "GameFramework/Character.h"
#include "StatusEffectData.h"
#include "PlayerSurvivalStatComponent.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h" // UStruct(FDotEffectParams)를 JSON으로 변환/파싱하는 핵심 기능
#include "SpecialAbilityEffectClass.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API USpecialAbilityEffectClass : public UObject, public ISetEfectInterface
{
	GENERATED_BODY()
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ACharacter> AffectedCharacter; // 대상 캐릭터 (컴포넌트에서 주입)
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName m_EffectID; // 효과의 이름
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_EffectDuration; // 효과 지속 시간
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_EffectStatValue; // 효과 정도

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_CanLock;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_CanAttack;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_CanJump;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_CanSprint;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_Cpylock;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_Cpyattack;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_Cpyjump;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool m_Cpysprint;
public:
	// JSON 문자열을 받아 파싱하여 내부 변수에 설정
	void InitializeEffectData(ACharacter* Target, const FString& ParamsJson);
	// 인터페이스 구현 (매개변수 없음)
	virtual void ExecuteEffect_Implementation() override;

	// 인터페이스 구현 (제거 로직)
	virtual void CleanUpEffect_Implementation() override;
};
