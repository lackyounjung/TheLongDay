// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusWidgetBase.h"
#include "StatusIconWidget.h"
#include "Components/ScrollBox.h"
#include "PlayerEffectComponent.h"
#include "StatusEffectData.h"
#include "Kismet/GameplayStatics.h"

void UStatusWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // 1. 컴포넌트 찾아오기
    APawn* PlayerPawn = GetOwningPlayerPawn();
    if (PlayerPawn)
    {
        UPlayerEffectComponent* EffectComp = PlayerPawn->FindComponentByClass<UPlayerEffectComponent>();
        if (EffectComp)
        {
            // 2. 델리게이트 바인딩
            EffectComp->OnStatusChanged.AddDynamic(this, &UStatusWidgetBase::OnStatusUpdate);

            // 초기화를 위해 한 번 실행
            RefreshStatusIcons();
        }
    }
}

void UStatusWidgetBase::OnStatusUpdate()
{
    RefreshStatusIcons();
}

void UStatusWidgetBase::RefreshStatusIcons()
{
    if (!StatusScrollBox || !IconWidgetClass) return;

    // 1. 기존 아이콘 제거
    StatusScrollBox->ClearChildren();

    // 2. 플레이어 컴포넌트 참조 가져오기
    APawn* PlayerPawn = GetOwningPlayerPawn();
    if (!PlayerPawn) return;

    UPlayerEffectComponent* EffectComp = PlayerPawn->FindComponentByClass<UPlayerEffectComponent>();
    if (!EffectComp) return;

    // 3. 데이터 테이블 가져오기
    UDataTable* StatTable = EffectComp->GetStatusDataTable();
    if (!StatTable) return;

    // 4. 현재 적용 중인 효과들 순회
    const TMap<FName, FActiveEffect>& ActiveEffects = EffectComp->GetActiveEffects();

    for (const auto& Pair : ActiveEffects)
    {
        const FName& EffectID = Pair.Key;
        const FActiveEffect& ActiveData = Pair.Value;

        // 5. 데이터 테이블에서 시각적 정보(DisplayName, Icon 등) 조회
        const FEffectData* RowData = StatTable->FindRow<FEffectData>(EffectID, TEXT("UI_Refresh"));

        if (RowData)
        {
            // 6. 아이콘 위젯 생성
            UStatusIconWidget* NewIcon = CreateWidget<UStatusIconWidget>(this, IconWidgetClass);
            if (NewIcon)
            {
                // 7. 위젯에 데이터 전달 (ID, 이름, 아이콘, 최대시간, 현재남은시간)
                NewIcon->InitIcon(
                    EffectID,
                    RowData->DisplayName,
                    RowData->IconTexture,
                    RowData->DurationSeconds,
                    EffectComp
                );

                // 8. 스크롤 박스에 추가
                StatusScrollBox->AddChild(NewIcon);
            }
        }
    }
}
