// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAbilityEffectClass.h"
#include "TheLongDarkPlayer.h"

void USpecialAbilityEffectClass::InitializeEffectData(ACharacter* Target, const FString& ParamsJson)
{
    AffectedCharacter = Target;
    // 1.JSON 문자열을 FDotEffectParams 구조체로 파싱 시도 
    FSpecialAbilityEffectParams ParsedParams;

    // FJsonObjectConverter를 사용하여 JSON 문자열을 USTRUCT(FDotEffectParams)로 변환
    if (FJsonObjectConverter::JsonObjectStringToUStruct(ParamsJson, &ParsedParams, 0, 0))
    {
        // 2. 파싱 성공 시, 파라미터 구조체의 값들을 멤버 변수에 저장
        

        
        m_CanLock = ParsedParams.Canlock;
        m_CanAttack = ParsedParams.CanAttack;
        m_CanJump = ParsedParams.CanJump;
        m_CanSprint = ParsedParams.CanSprint;


    }
    else
    {
        // 3. 파싱 실패 시 경고
        UE_LOG(LogTemp, Error, TEXT("DOT Init Failed: Failed to parse JSON parameters for Effect. JSON String: %s"), *ParamsJson);

        // 실패 시 안전을 위해 기본값이나 0을 설정하는 것이 좋습니다.
        m_EffectStatValue = 0.0f;
    }
}

void USpecialAbilityEffectClass::ExecuteEffect_Implementation()
{
    ACharacter* Target = AffectedCharacter.Get();
    if (Target)
    {
        ATheLongDarkPlayer* player = Cast<ATheLongDarkPlayer>(Target);
        if (player)
        {
            m_Cpyattack = player->GetCanAttack();
            m_Cpylock = player->GetCanLock();
            m_Cpyjump = player->GetCanJump();
            m_Cpysprint = player->GetCanSprint();
            player->SetCanAttack(m_CanAttack);
            player->SetCanLock(m_CanLock);
            player->SetCanJump(m_CanJump);
            player->SetCanSprint(m_CanSprint);
        }
    }
}

void USpecialAbilityEffectClass::CleanUpEffect_Implementation()
{
    ACharacter* Target = AffectedCharacter.Get();
    if (Target)
    {
        ATheLongDarkPlayer* player = Cast<ATheLongDarkPlayer>(Target);
        if (player)
        {

            player->SetCanAttack(m_Cpyattack);
            player->SetCanLock(m_Cpylock);
            player->SetCanJump(m_Cpyjump);
            player->SetCanSprint(m_Cpysprint);
        }
    }
}
