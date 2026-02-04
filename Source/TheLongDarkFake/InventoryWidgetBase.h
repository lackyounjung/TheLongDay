// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidgetBase.h"
#include "Item_Data.h"
#include "InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "UseItempopup.h"
#include "InventoryWidgetBase.generated.h"


/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
private:
    UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    int m_slotNumber =100;
    UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    float m_Inventoryweight = 0.00f;
    UPROPERTY(BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    float m_InventoryMaxweight = 100.00f;
protected:
    // 3. Native Construct에서 레이아웃 설정을 호출하는 것이 일반적입니다.
    virtual void NativeConstruct() override;

public:
    // 1. UMG에서 Uniform Grid Panel을 연결할 변수 추가 (Scroll Box 안에 있는 패널)
    UPROPERTY(meta = (BindWidget))
    class UUniformGridPanel* SlotGrid;
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "Inventory")
    UUseItempopup* WBP_UseItempopup;

    // 인벤토리 컴포넌트와의 연결 (BeginPlay 등에서 설정)
    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    TWeakObjectPtr<UInventoryComponent> OwnerInventoryComponent;

    // 인벤토리의 모든 슬롯 위젯 목록 (블루프린트에서 채워집니다)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Inventory")
    TArray<USlotWidgetBase*> SlotWidgets;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Settings")
    TSubclassOf<USlotWidgetBase> SlotWidgetClass;

    // 2. 인벤토리 컴포넌트로부터 데이터를 가져와 UI를 새로고침하는 함수 (핵심)
    UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
    void RefreshInventoryUI();

    UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
    void RefreshInventoryUIbyType(EItemType Itemtype);

    UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
    void SetupSlotsLayout();

    UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
    void SetupItemPopup(const FItem_Data& NewItemData);

    

    
};
