// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTargetWidget.h"
#include "Components/TextBlock.h"

void UQuestTargetWidget::UpdateTarget(FText TargetName, int32 Current, int32 Max)
{
    if (TargetNameText)
    {
        TargetNameText->SetText(TargetName);
    }

    if (TargetProgressText)
    {
        // "3 / 5" 형식으로 문자열 조립
        FText Progress = FText::Format(FText::FromString("{0} / {1}"), FText::AsNumber(Current), FText::AsNumber(Max));
        TargetProgressText->SetText(Progress);

        // 목표 달성 시 색상을 초록색으로 바꾸는 센스!
        if (Current >= Max)
        {
            TargetProgressText->SetColorAndOpacity(FLinearColor::Green);
        }
        else
        {
            TargetProgressText->SetColorAndOpacity(FLinearColor::White);
        }
    }
}
