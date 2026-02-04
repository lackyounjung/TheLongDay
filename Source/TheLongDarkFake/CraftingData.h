// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CraftingData.generated.h" // USTRUCT/UENUM 정의가 여기에 포함됩니다.

// 1. 보조 구조체: 재료 항목 정의
USTRUCT(BlueprintType)
struct FCraftingIngredient
{
	GENERATED_BODY()
public:
	// DT_Item_Master를 참조할 아이템의 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredient")
	FName ItemID;

	// 이 아이템이 몇 개 필요한지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ingredient")
	int32 Quantity = 1;
};

// 2. 메인 구조체: 제작 레시피 정의
USTRUCT(BlueprintType)
struct FCraftingRecipe : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 1. 식별 및 표시
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FName RecipeID; // 레시피 고유 ID (데이터 테이블 Row Name과 일치 권장)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
	FText DisplayName; // 제작 목록에 표시될 이름 (예: "붕대 제작")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Output")
	//TSoftObjectPtr<UTexture2D> IconTexture;
	UTexture2D* IconTexture;

	// 2. 결과물 (Output)
	// DT_Item_Master를 참조할 결과 아이템의 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Output")
	FName ResultItemID;

	// 제작 시 결과 아이템이 몇 개 나오는지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Output")
	int32 ResultQuantity = 1;

	// 3. 재료 목록 (Input)
	// TArray<FCraftingIngredient>를 사용합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<FCraftingIngredient> Ingredients;

	// 4. 제작 조건 및 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
	float CraftingTimeSeconds = 3.0f; // 제작에 소요되는 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
	int32 RequiredSkillLevel = 0; // 필요한 최소 스킬 레벨 (확장 시)

	// 제작을 할 수 있는 환경 조건 (예: 모닥불, 작업대 등)
	// DT_Interactable_Object 또는 별도 Station 테이블 ID를 참조
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition")
	FName RequiredStationID;

	
};
