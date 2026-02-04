// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeWidgetBase.h"
#include "Components/TextBlock.h"

void UTimeWidgetBase::UpdateTime(float InCurrentTime)
{
	// 1. 시간(Hour) 추출: 소수점 버림
	int32 Hours = FMath::FloorToInt(InCurrentTime);

	// 2. 분(Minute) 추출: 소수점 아래 부분에 60을 곱함
	float MinuteFraction = FMath::Fmod(InCurrentTime, 1.0f);
	int32 Minutes = FMath::FloorToInt(MinuteFraction * 60.0f);

	// 3. 최적화: 분 단위가 바뀌었을 때만 텍스트 갱신
	if (Minutes != LastDisplayedMinute)
	{
		LastDisplayedMinute = Minutes;

		// 24시간 형식 포맷팅 (예: 8, 5 -> "08 : 05")
		FString TimeString = FString::Printf(TEXT("%02d : %02d"), Hours, Minutes);

		if (TimeText)
		{
			TimeText->SetText(FText::FromString(TimeString));
		}

		// 4. (보너스) 오전/오후 표시 로직
		if (AMPMText)
		{
			FText AMPM = (Hours < 12) ? FText::FromString(TEXT("AM")) : FText::FromString(TEXT("PM"));
			AMPMText->SetText(AMPM);
		}
	}
}
