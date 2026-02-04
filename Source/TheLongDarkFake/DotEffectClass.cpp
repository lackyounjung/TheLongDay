// Fill out your copyright notice in the Description page of Project Settings.


#include "DotEffectClass.h"
#include "MovementEffectClass.h"

void UDotEffectClass::ApplyDamageTick()
{
    // TWeakObjectPtr에서 포인터를 안전하게 가져오고 유효성 검사
    if (ACharacter* Target = AffectedCharacter.Get())
    {
        // 1. 대상 캐릭터로부터 UPlayerSurvivalStatComponent를 찾습니다.
        UPlayerSurvivalStatComponent* StatComp = Target->FindComponentByClass<UPlayerSurvivalStatComponent>();

        if (StatComp)
        {
            // 2. DOT 효과 값(m_EffectStatValue)을 스탯 컴포넌트의 함수에 전달합니다.
            //    m_EffectStatValue는 DOT 피해량이므로, 예를 들어 1.0f라면
            //    ChangeStatValue는 1.0을 빼야 하므로 값 자체를 전달합니다.

            //    주의: DOT는 스탯을 '감소'시켜야 하므로, m_EffectStatValue가 양수(예: 5.0)일 경우
            //    함수 호출 시 **음수(-5.0)**를 전달해야 스탯이 감소합니다.
            //    만약 데이터 테이블에서 m_EffectStatValue를 이미 음수로 설정했다면 그대로 전달합니다.

            float DamageAmount = m_EffectStatValue; // 예시: m_EffectStatValue가 5.0이면 -5.0을 전달

            StatComp->ChangeStatValue(m_statID, DamageAmount);

            UE_LOG(LogTemp, Display,
                TEXT("DOT Ticked on %s: Changed Stat '%s' by %.2f"),
                *Target->GetName(),
                *m_statID.ToString(),
                DamageAmount);
        }
        else
        {
            UE_LOG(LogTemp, Error,
                TEXT("DOT Tick Failed: Target character '%s' is missing PlayerSurvivalStatComponent."),
                *Target->GetName());
        }
    }
    else
    {
        // 대상 캐릭터가 사라졌다면, 스스로 정리합니다.
        CleanUpEffect_Implementation();
    }
}



void UDotEffectClass::InitializeEffectData(ACharacter* Target, const FString& ParamsJson)
{
    AffectedCharacter = Target;
    // 1.JSON 문자열을 FDotEffectParams 구조체로 파싱 시도 
    FDotEffectParams ParsedParams;

    // FJsonObjectConverter를 사용하여 JSON 문자열을 USTRUCT(FDotEffectParams)로 변환
    if (FJsonObjectConverter::JsonObjectStringToUStruct(ParamsJson,&ParsedParams,0,0))
    {
        // 2. 파싱 성공 시, 파라미터 구조체의 값들을 멤버 변수에 저장
        m_statID = ParsedParams.TargetStatID;
        m_EffectInterval = ParsedParams.TickInterval;
        m_EffectStatValue = ParsedParams.DotEffectAmount;

        // 디버그 로그
        UE_LOG(LogTemp, Log, TEXT("DOT Init Success: Stat=%s, Interval=%.2f, Value=%.2f"),
            *m_statID.ToString(), m_EffectInterval, m_EffectStatValue);
    }
    else
    {
        // 3. 파싱 실패 시 경고
        UE_LOG(LogTemp, Error, TEXT("DOT Init Failed: Failed to parse JSON parameters for Effect. JSON String: %s"), *ParamsJson);

        // 실패 시 안전을 위해 기본값이나 0을 설정하는 것이 좋습니다.
        m_statID = "None";
        m_EffectInterval = 0.0f;
        m_EffectStatValue = 0.0f;
    }
}



void UDotEffectClass::ExecuteEffect_Implementation()
{
    if (ACharacter* Target = AffectedCharacter.Get())
    {
        // 1초 이하의 너무 짧은 주기는 피하거나, 최소값을 설정하는 로직 추가 가능
        if (m_EffectInterval <= 0.0f)
        {
            m_EffectInterval = 0.5f; // 안전을 위한 기본값 설정
        }

        // 월드 타이머 매니저를 사용하여 ApplyDamageTick 함수를 반복 호출하도록 설정
        Target->GetWorldTimerManager().SetTimer(
            DamageTimerHandle,
            this,
            &UDotEffectClass::ApplyDamageTick, // UFUNCTION으로 등록된 함수
            m_EffectInterval,
            true // 반복 (Looping)
        );

        UE_LOG(LogTemp, Warning, TEXT("DOT Effect Applied: Target=%s, Timer Started."), *Target->GetName());

       
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to apply DOT effect: Target character is null or invalid."));
    }
}

void UDotEffectClass::CleanUpEffect_Implementation()
{
    if (ACharacter* Target = AffectedCharacter.Get())
    {
        // 타이머를 해제하여 더 이상 ApplyDamageTick이 호출되지 않도록 합니다.
        Target->GetWorldTimerManager().ClearTimer(DamageTimerHandle);

        UE_LOG(LogTemp, Warning, TEXT("DOT Effect Removed/Cleaned Up on: %s"), *Target->GetName());

        //주의: 스탯 디버프였다면 이 곳에서 스탯 모디파이어를 제거하는 로직이 추가됩니다.
    }
}


