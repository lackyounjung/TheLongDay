// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementEffectClass.h"
#include "TheLongDarkPlayer.h"


void UMovementEffectClass::InitializeEffectData(ACharacter* Target, const FString& ParamsJson)
{
    AffectedCharacter = Target;
    // 1.JSON 문자열을 FDotEffectParams 구조체로 파싱 시도 
    FMoveMentEffectParams ParsedParams;

    // FJsonObjectConverter를 사용하여 JSON 문자열을 USTRUCT(FDotEffectParams)로 변환
    if (FJsonObjectConverter::JsonObjectStringToUStruct(ParamsJson, &ParsedParams, 0, 0))
    {
        // 2. 파싱 성공 시, 파라미터 구조체의 값들을 멤버 변수에 저장
        
        
        m_EffectStatValue = ParsedParams.MoveEffectAmount;

        
    }
    else
    {
        // 3. 파싱 실패 시 경고
        UE_LOG(LogTemp, Error, TEXT("DOT Init Failed: Failed to parse JSON parameters for Effect. JSON String: %s"), *ParamsJson);

        // 실패 시 안전을 위해 기본값이나 0을 설정하는 것이 좋습니다.
        m_EffectStatValue = 0.0f;
    }
}

void UMovementEffectClass::ExecuteEffect_Implementation()
{
    ACharacter* Target = AffectedCharacter.Get();
    if (Target)
    {
        ATheLongDarkPlayer* player = Cast<ATheLongDarkPlayer>(Target);
        if (player)
        {
            m_CpySpd = player->GetMovementSpeed();
            player->SetMovementSpeed(m_EffectStatValue);
        }
    }

}

void UMovementEffectClass::CleanUpEffect_Implementation()
{
    ACharacter* Target = AffectedCharacter.Get();
    if (Target)
    {
        ATheLongDarkPlayer* player = Cast<ATheLongDarkPlayer>(Target);
        if (player)
        {
            
            player->SetMovementSpeed(m_CpySpd);
        }
    }
}
