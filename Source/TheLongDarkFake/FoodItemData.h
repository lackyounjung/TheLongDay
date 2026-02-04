// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Data.h"
#include "FoodItemData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FFoodItemData : public FItem_Data
{
	GENERATED_BODY()
public:
    // 음식에만 필요한 생존 상태 변화 속성
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Stats")
    float HealthRestored = 0.0f; // 체력 회복량

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Stats")
    float HungerSatiety = 0.0f; // 포만감 회복량 (배고픔 감소)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Stats")
    float ThirstChange = 0.0f; // 갈증 변화량 (음료수면 +, 건조한 음식이면 -)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Food Stats")
    bool bIsCooked = false; // 요리가 필요한지 여부
};
