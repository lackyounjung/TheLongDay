// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item_Data.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "UseItempopup.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UUseItempopup : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItem_Data ItemData;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Item Id ")
	UTextBlock* TextBlock_ItemName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Item Id ")
	UTextBlock* TextBlock_Description;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Item Id ")
	TObjectPtr<UImage> Image_Item;

	//데이터 연결할 함수
	
	void SetDatainTextAndImage(const FItem_Data& NewItemData);
	//인터페이스로 버튼 누를 시 발동할 함수 가져오기
	
};
