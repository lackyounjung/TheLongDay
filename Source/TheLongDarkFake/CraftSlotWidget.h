// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingData.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "CraftSlotWidget.generated.h"

class UCraftUIWidget;
/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UCraftSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // 슬롯에 표시할 데이터
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot Data")
    FCraftingRecipe CraftRecipeData;

    // 슬롯이 현재 비어있는지 여부
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slot Data")
    bool bIsEmpty = true;
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Slot Image")
    TObjectPtr<UImage> RecipeImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Slot text")
    UTextBlock* RecipeTitle;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "inventorywidget")
    UCraftUIWidget* CraftUIWidget;

    UFUNCTION(BlueprintCallable, Category = "Slot Functions")
    void SetCraftSlotData(const FCraftingRecipe& NewRecipe, bool bNewIsEmpty);



    

    UFUNCTION(BlueprintCallable, Category = "Use Slot Item Data")
    void UseCraftSlotItem(const FCraftingRecipe& NewRecip );

    
};
