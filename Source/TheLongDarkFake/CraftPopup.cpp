// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftPopup.h"
#include "Kismet/KismetMathLibrary.h" // 행/열 계산을 위해 필요
#include "Components/UniformGridSlot.h" // 그리드 슬롯 추가를 위해 필요
#include "Engine/Engine.h"

bool UCraftPopup::SetDataInPopup(const FCraftingRecipe& RecipeData)
{

    m_Recipe = RecipeData;

	FText InId = FText::FromName(RecipeData.RecipeID);
	RecipeID->SetText(InId);

	Description->SetText( RecipeData.DisplayName);

	float TimeValue = RecipeData.CraftingTimeSeconds;
	const FText FormatPattern = NSLOCTEXT("ItemFormat", "WeightUnit", "{0}MINUTES");
	FText InText = FText::Format(FormatPattern, FText::AsNumber(TimeValue));
	CraftingTime->SetText(InText);
    SetupCraftSlotLayout(RecipeData);

	return false;
}

void UCraftPopup::SetupCraftSlotLayout(const FCraftingRecipe& RecipeData)
{
    if (!NeadIngredientsGrid)
    {
        return;
    }
    if (!CraftPopupSlotWidgetClass)
    {
        return;
    }

    const int32 Columns = RecipeData.Ingredients.Num();

    NeadIngredientsGrid->ClearChildren();
    IngredientsSlots.Empty();

    for (int32 i = 0; i < Columns; i++)
    {
        // 3-1. 행과 열 계산
        int32 Row = i / Columns;
        int32 Column = i;

        // 3-2. 슬롯 위젯 생성
        UIngredientsSlotWidget* NewSlotWidget = CreateWidget<UIngredientsSlotWidget>(this, CraftPopupSlotWidgetClass);
        
        

        if (!NewSlotWidget)
        {
            
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("Failed to create craft Slot Widget."), false);
            continue;
        }
        else
        {
            
            NewSlotWidget->SetData(RecipeData.Ingredients[i]); //여기가 문제
        }

        

        // 3-3. 위젯을 그리드 패널에 추가 (Uniform Grid Slot 반환)
        UUniformGridSlot* GridSlot = NeadIngredientsGrid->AddChildToUniformGrid(NewSlotWidget, Row, Column);

        // 3-4. (선택) 그리드 슬롯의 정렬(Alignment) 설정
        if (GridSlot)
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
        }

        // 3-5. 생성된 슬롯 위젯 목록에 저장
        IngredientsSlots.Add(NewSlotWidget);

        // 3-6. (옵션) 슬롯에 인덱스 또는 관련 데이터 설정 (예: NewSlotWidget->SlotIndex = i;)
    }
}

FCraftingRecipe UCraftPopup::GetRecipe()
{
    return m_Recipe;
}