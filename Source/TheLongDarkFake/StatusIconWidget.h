// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UStatusIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    /**
     * @param InEffectID 효과의 고유 ID
     * @param InDisplayName 데이터 테이블에 정의된 FText 이름
     * @param InIconTexture SoftObjectPtr로 저장된 아이콘 텍스처
     */
    void InitIcon(FName InEffectID, FText InDisplayName, TSoftObjectPtr<UTexture2D> InIconTexture, float InMaxDuration, class UPlayerEffectComponent* InOwnerComp);
    
protected:
    // 매 프레임 호출되는 Tick
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void UpdateVisualTime(float InCurrentTime);
    // 블루프린트의 Image 컴포넌트 (아이콘)
    UPROPERTY(meta = (BindWidget))
    class UImage* IconImage;

    // 블루프린트의 TextBlock 컴포넌트 (효과 이름 표시용)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* EffectNameText;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* DurationBar; // 블루프린트에서 ProgressBar 이름을 이렇게 지어주세요.

    float MaxDuration;

    // 현재 위젯이 관리하는 효과 ID
    UPROPERTY(VisibleAnywhere, Category = "Status")
    FName TargetEffectID;

    // 가비지 컬렉션으로부터 안전하게 컴포넌트를 참조 (약포인터)
    TWeakObjectPtr<class UPlayerEffectComponent> OwnerComponent;
	
};
