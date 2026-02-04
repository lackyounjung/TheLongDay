// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item_Data.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SlotWidgetBase.generated.h"

class UInventoryWidgetBase;


/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API USlotWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
    // 슬롯에 표시할 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot Data")
    FItem_Data ItemData;

    // 슬롯이 현재 비어있는지 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot Data")
    bool bIsEmpty = true;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Slot Image")
    TObjectPtr<UImage> SlotImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Slot text")
    UTextBlock* kgtext;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "inventorywidget")
    UInventoryWidgetBase* InventoryWidget;
    
    



    // 슬롯에 데이터를 설정하고, 블루프린트 UI 업데이트를 요청하는 함수
    UFUNCTION(BlueprintCallable, Category = "Slot Functions")
    void SetSlotData(const FItem_Data& NewItemData, bool bNewIsEmpty);

    

    // 슬롯 데이터가 변경되었을 때, 블루프린트에서 UI 요소를 업데이트하기 위한 이벤트 함수
    UFUNCTION(BlueprintCallable, Category = "Slot Functions")
    void OnSlotUpdated();

    UFUNCTION(BlueprintCallable, Category = "Use Slot Item Data")
    void UseSlotItem();

    // 아이템 사용 등 상호작용을 처리하는 가상 함수 (BP 오버라이드 가능)
    // 예: 마우스 오른쪽 클릭 시 사용 메뉴 띄우기
    //UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Slot Functions")
    //void OnItemUseRequested();
    //virtual void OnItemUseRequested_Implementation() {};
};
