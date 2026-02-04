// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftUIWidget.h"
#include "Components/UniformGridSlot.h" // 그리드 슬롯 추가를 위해 필요
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h" // 행/열 계산을 위해 필요

void UCraftUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetupCraftSlotLayout();
	UCraftingComponent* CraftingComponent = OwnerCraftingComponent.Get();

	if (CraftingComponent)
	{
        CraftingComponent->FOnCraftInventoryChange.AddDynamic(this, &UCraftUIWidget::RefreshCraftUI);
        RefreshCraftUI();
	}
	else
	{
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("Ready to refreshcraftinventory"), false);
        return;
	}
}

void UCraftUIWidget::RefreshCraftUI()
{
    UCraftingComponent* CraftingComponent = OwnerCraftingComponent.Get();
    int RecipeNum = CraftingComponent->GetRecipes().Num();
    SetupCraftSlotLayout();

   
    for (int i = 0; i < RecipeNum; i++)
    {
        TArray<FCraftingRecipe>Recipesdatas = CraftingComponent->GetRecipes();
        

        CraftSlots[i]->SetCraftSlotData(Recipesdatas[i], true);

        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses refreshinventory"), false);
    }
}

void UCraftUIWidget::SetupCraftSlotLayout()
{
    if (!CraftSlotGrid)
    {
        return;
    }
    if (!CraftSlotWidgetClass)
    {
        return;
    }

    const int32 Columns = 4;

    CraftSlotGrid->ClearChildren();
    CraftSlots.Empty();

    for (int32 i = 0; i < m_craftslotnum; i++)
    {
        // 3-1. 행과 열 계산
        int32 Row = i / Columns;
        int32 Column = i % Columns;

        // 3-2. 슬롯 위젯 생성
        UCraftSlotWidget* NewSlotWidget = CreateWidget<UCraftSlotWidget>(this, CraftSlotWidgetClass);
        NewSlotWidget->CraftUIWidget = this;
        //NewSlotWidget->UseCraftSlotItem();

        if (!NewSlotWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create craft Slot Widget."));
            continue;
        }

        // 3-3. 위젯을 그리드 패널에 추가 (Uniform Grid Slot 반환)
        UUniformGridSlot* GridSlot = CraftSlotGrid->AddChildToUniformGrid(NewSlotWidget, Row, Column);

        // 3-4. (선택) 그리드 슬롯의 정렬(Alignment) 설정
        if (GridSlot)
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
        }

        // 3-5. 생성된 슬롯 위젯 목록에 저장
        CraftSlots.Add(NewSlotWidget);

        // 3-6. (옵션) 슬롯에 인덱스 또는 관련 데이터 설정 (예: NewSlotWidget->SlotIndex = i;)
    }
    
}

void UCraftUIWidget::SetupCraftPopup(const FCraftingRecipe& newRecipe)
{
    WBP_CraftingPopup->SetDataInPopup(newRecipe);
}
