// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetBase.h"
#include "Components/UniformGridSlot.h" // 그리드 슬롯 추가를 위해 필요
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h" // 행/열 계산을 위해 필요


void UInventoryWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // 슬롯 레이아웃 설정을 여기서 호출하거나, 인벤토리 컴포넌트 설정 후 호출합니다.
    SetupSlotsLayout();
    UInventoryComponent* InventoryComponent = OwnerInventoryComponent.Get();

    if (InventoryComponent)
    {
        // OwnerInventoryComponent의 OnInventoryChanged 델리게이트에
        // 이 위젯 인스턴스의 RefreshInventoryUI 함수를 바인딩합니다.
        InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidgetBase::RefreshInventoryUI);

        // 델리게이트 바인딩 후, 초기 UI 새로고침을 수행하는 것이 좋습니다.
        RefreshInventoryUI();
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("inventoryfalse "), false);
    }
    
}

void UInventoryWidgetBase::RefreshInventoryUI()
{
    UInventoryComponent* Inventorycomponent = OwnerInventoryComponent.Get();
    int itemsNum = Inventorycomponent->GetInventory().Num();
    SetupSlotsLayout();
    
    m_Inventoryweight = 0;
    for (int i = 0; i < itemsNum; i++)
    {
        TArray<FItem_Data>inventorydatas = Inventorycomponent->GetInventory();
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses refreshinventory"), false);

        SlotWidgets[i]->SetSlotData(inventorydatas[i], true);
        m_Inventoryweight += inventorydatas[i].Weight;
    }
}

void UInventoryWidgetBase::RefreshInventoryUIbyType(EItemType Itemtype)
{
    UInventoryComponent* Inventorycomponent = OwnerInventoryComponent.Get();
    int itemsNum = Inventorycomponent->GetInventory().Num();
    
    SetupSlotsLayout();

    int slotsNum = 0;

    for (int i = 0; i < itemsNum; i++)
    {
        TArray<FItem_Data>inventorydatas = Inventorycomponent->GetInventory();
        
        

        if (inventorydatas[i].ItemType == Itemtype)
        {
            SlotWidgets[slotsNum]->SetSlotData(inventorydatas[i], true);
            slotsNum += 1;
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses refreshinventorybytype"), false);
        }
    }


}

void UInventoryWidgetBase::SetupSlotsLayout()
{
    // 1. 필요한 필수 요소 확인
    if (!SlotGrid)
    {
        UE_LOG(LogTemp, Error, TEXT("SlotGrid is null. Did you bind the UMG widget correctly?"));
        return;
    }
    if (!SlotWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("SlotWidgetClass is null. Set the slot widget BP in InventoryWidgetBase BP."));
        return;
    }

    // 2. 그리드 열 수 결정 (인벤토리 디자인에 따라 다름, 예: 5개)
    const int32 Columns = 10;

    // 이전에 생성된 슬롯이 있다면 제거 (재배치/새로고침 시)
    SlotGrid->ClearChildren();
    SlotWidgets.Empty();

    // 3. m_slotNumber 만큼 슬롯 생성 및 그리드에 추가
    for (int32 i = 0; i < m_slotNumber; ++i)
    {
        // 3-1. 행과 열 계산
        int32 Row = i / Columns;
        int32 Column = i % Columns;

        // 3-2. 슬롯 위젯 생성
        USlotWidgetBase* NewSlotWidget = CreateWidget<USlotWidgetBase>(this, SlotWidgetClass);
        NewSlotWidget->InventoryWidget = this;
        NewSlotWidget->UseSlotItem();

        if (!NewSlotWidget)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create Slot Widget."));
            continue;
        }

        // 3-3. 위젯을 그리드 패널에 추가 (Uniform Grid Slot 반환)
        UUniformGridSlot* GridSlot = SlotGrid->AddChildToUniformGrid(NewSlotWidget, Row, Column);

        // 3-4. (선택) 그리드 슬롯의 정렬(Alignment) 설정
        if (GridSlot)
        {
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
        }

        // 3-5. 생성된 슬롯 위젯 목록에 저장
        SlotWidgets.Add(NewSlotWidget);

        // 3-6. (옵션) 슬롯에 인덱스 또는 관련 데이터 설정 (예: NewSlotWidget->SlotIndex = i;)
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully created %d slots and added to Uniform Grid Panel."), SlotWidgets.Num());
}

void UInventoryWidgetBase::SetupItemPopup(const FItem_Data& NewItemData)
{
    WBP_UseItempopup->SetDatainTextAndImage(NewItemData);
}

