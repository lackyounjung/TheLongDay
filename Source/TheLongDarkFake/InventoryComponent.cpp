// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemBase.h"
#include "UseitemInterface.h"
#include "QuestComponent.h"

#include "Engine/Engine.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UInventoryComponent::AddItem(FName ItemID)
{
    if (ItemID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem failed: ItemID is None."));
        return false;
    }

    UQuestComponent* questcomp = GetOwner()->FindComponentByClass<UQuestComponent>();
    questcomp->NotifyAction(EQuestObjectiveType::Gather, ItemID);

    // ----------------------------------------------------
    // 1. ItemMasterTable (기본 데이터)에서 아이템 검색
    // ----------------------------------------------------
    if (ItemMasterTable)
    {
        // ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
        if (const FItem_Data* MasterRow = ItemMasterTable->FindRow<FItem_Data>(ItemID, TEXT("Find Master Item Data")))
        {
            // 기본 아이템 데이터 찾음
            InventoryItemsData.Add(*MasterRow);
            OnInventoryChanged.Broadcast();
            OnAddItem.Broadcast(ItemID);
            return true;
        }
        
            
    }
    if (GearItemDataTable)
    {
        // ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
        if (const FGearItemData* MasterRow = GearItemDataTable->FindRow<FGearItemData>(ItemID, TEXT("Find GearItemData")))
        {
            // 기본 아이템 데이터 찾음
            InventoryItemsData.Add(*MasterRow);
            OnInventoryChanged.Broadcast();
            OnAddItem.Broadcast(ItemID);
            return true;
        }
    }
    if (FoodItemDataTable)
    {
        // ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
        if (const FFoodItemData* MasterRow = FoodItemDataTable->FindRow<FFoodItemData>(ItemID, TEXT("Find FoodItemDataTable")))
        {
            // 기본 아이템 데이터 찾음
            InventoryItemsData.Add(*MasterRow);
            OnInventoryChanged.Broadcast();
            OnAddItem.Broadcast(ItemID);
            return true;
        }
    }

    // ----------------------------------------------------
    // 3. 아이템을 어떤 테이블에서도 찾지 못한 경우
    // ----------------------------------------------------
   
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AddItem failed: Item data not found in any DataTable for ItemID: %s"), *ItemID.ToString());
        return false;
    }
        
    
    return false;
	
}

void UInventoryComponent::RemoveItem(const FItem_Data& ItemData)
{
    const int32 FoundIndex = InventoryItemsData.IndexOfByPredicate([&ItemData](const FItem_Data& Currentitem) {return Currentitem.ItemID == ItemData.ItemID; });
    if (FoundIndex != INDEX_NONE)
    {
        InventoryItemsData.RemoveAt(FoundIndex);
        OnInventoryChanged.Broadcast();
    }
    
}

void UInventoryComponent::UseItem(FItem_Data ItemData)
{
    if (ItemData.ItemType == EItemType::EIT_Equipment || ItemData.ItemType == EItemType::EIT_BuildItem)
    {
        if (const FGearItemData* MasterRow = GearItemDataTable->FindRow<FGearItemData>(ItemData.ItemID, TEXT("Find GearItemData")))
        {
            AActor* Item = GetWorld()->SpawnActor<AActor>(MasterRow->ItemActorClass);
            // 2. Item의 클래스가 IUseitemInterface를 구현했는지 확인
            if (Item->GetClass()->ImplementsInterface(UUseitemInterface::StaticClass()))
            {
                // 구현했다면 안전하게 호출
                IUseitemInterface::Execute_UseItemdata(Item, GetOwner());

                Item->Destroy();

                // 아이템 사용 로직이 끝난 후 인벤토리에서 제거
                if (ItemData.ItemType == EItemType::EIT_Equipment)
                {
                    RemoveItem(ItemData);
                }
                else if (ItemData.ItemType == EItemType::EIT_BuildItem)
                {

                }
                
            }
            else
            {
                // 아이템이 인터페이스를 구현하지 않았다면,
                // 크래시를 방지하기 위해 생성된 Item을 바로 Destroy하고 리턴하거나
                // 다음 로직으로 건너뜁니다.

                // 생성했지만 사용하지 못했으니 파괴합니다.
                Item->Destroy();

                // 이 함수를 즉시 종료하고 싶다면 여기서 return을 사용하세요.
                //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("cant use"), false);
                return;
            }
        }

    }
    else if (ItemData.ItemType == EItemType::EIT_Resource)
    {
        if (const FItem_Data* MasterRow = ItemMasterTable->FindRow<FItem_Data>(ItemData.ItemID, TEXT("Find ResourceItemData")))
        {
            AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(MasterRow->ItemActorClass);
            

            // 2. Item의 클래스가 IUseitemInterface를 구현했는지 확인
            if (Item->GetClass()->ImplementsInterface(UUseitemInterface::StaticClass()))
            {
                // 구현했다면 안전하게 호출
                IUseitemInterface::Execute_UseItemdata(Item, GetOwner());

                Item->Destroy();

                // 아이템 사용 로직이 끝난 후 인벤토리에서 제거
                RemoveItem(ItemData);
            }
            else
            {
                // 아이템이 인터페이스를 구현하지 않았다면,
                // 크래시를 방지하기 위해 생성된 Item을 바로 Destroy하고 리턴하거나
                // 다음 로직으로 건너뜁니다.

                // 생성했지만 사용하지 못했으니 파괴합니다.
                Item->Destroy();

                // 이 함수를 즉시 종료하고 싶다면 여기서 return을 사용하세요.
                //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("cant use"), false);
                return; 
            }

        }
    }
    else if (ItemData.ItemType == EItemType::EIT_Consumable)
    {
        if (const FFoodItemData* MasterRow = FoodItemDataTable->FindRow<FFoodItemData>(ItemData.ItemID, TEXT("Find FoodItemData")))
        {
            AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(MasterRow->ItemActorClass);
            if (Item)
            {
                IUseitemInterface::Execute_UseItemdata(Item, GetOwner());
                

                Item->Destroy();

                RemoveItem(ItemData);
            }
        }

    }
    else if (ItemData.ItemType == EItemType::EIT_KeyItem)
    {

    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses use food"), false);
    }
    
}

FItem_Data UInventoryComponent::Finditemdata(FName itemId)
{
    int inventorynum = InventoryItemsData.Num();
    for (int i = 0; i < inventorynum; i++)
    {
        if (InventoryItemsData[i].ItemID == itemId)
        {
            m_FindItemData = InventoryItemsData[i];
        }
    }
    return m_FindItemData;
}

FItem_Data* UInventoryComponent::FindItem(FName ItemID)
{
    int inventorynum = InventoryItemsData.Num();
    for (int i = 0; i < inventorynum; i++)
    {
        if (InventoryItemsData[i].ItemID == ItemID)
        {
            m_FindItemData = InventoryItemsData[i];
        }
            
    }
    return &m_FindItemData;
}



TArray<FItem_Data> UInventoryComponent::GetInventory()
{
	return InventoryItemsData;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

