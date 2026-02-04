// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusIconWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Engine/Texture2D.h"
#include "PlayerEffectComponent.h"

void UStatusIconWidget::InitIcon(FName InEffectID, FText InDisplayName, TSoftObjectPtr<UTexture2D> InIconTexture, float InMaxDuration, class UPlayerEffectComponent* InOwnerComp)
{
    TargetEffectID = InEffectID;
    MaxDuration = InMaxDuration;
    OwnerComponent = InOwnerComp;

    // 1. 디스플레이 이름 설정 (DisplayName)
    if (EffectNameText)
    {
        EffectNameText->SetText(InDisplayName);
    }

    // 2. 아이콘 이미지 설정 (SoftObjectPtr 처리)
    if (IconImage)
    {
        
        UTexture2D* LoadedTexture = InIconTexture.LoadSynchronous();


        if (LoadedTexture)
        {
            IconImage->SetBrushFromTexture(LoadedTexture);
        }
    }

    
}

void UStatusIconWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 1. 컴포넌트가 아직 유효한지 확인
    if (OwnerComponent.IsValid())
    {
        // 2. 컴포넌트의 ActiveEffects 맵에서 현재 이 아이콘의 데이터를 찾음
        const TMap<FName, FActiveEffect>& ActiveEffects = OwnerComponent->GetActiveEffects();

        if (ActiveEffects.Contains(TargetEffectID))
        {
            // 3. 실제 남은 시간을 가져와서 UI 갱신
            float RealTime = ActiveEffects[TargetEffectID].RemainingTime;
            UpdateVisualTime(RealTime);
        }
    }
}

void UStatusIconWidget::UpdateVisualTime(float InCurrentTime)
{
    if (DurationBar)
    {
        

        // 만약 영구 지속 효과(0.0f)라면 바를 가득 채우거나 숨깁니다.
        if (MaxDuration <= 0.0f)
        {
            DurationBar->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            DurationBar->SetVisibility(ESlateVisibility::Visible);
            // 비율 계산 (남은 시간 / 전체 시간)
            float Percent = FMath::Clamp(InCurrentTime / MaxDuration, 0.0f, 1.0f);
            DurationBar->SetPercent(Percent);
            UE_LOG(LogTemp, Log, TEXT("Effect %s Percent: %f"), *TargetEffectID.ToString(), Percent);
        }
    }
}
