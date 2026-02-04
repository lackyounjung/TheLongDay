// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CraftingData.h"
#include "IngredientsSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UIngredientsSlotWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	FCraftingIngredient m_IngredientData;
	float m_CurNum;
	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Ingredients Slot text")
	UTextBlock* IngredientsTitle;
	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "Ingredients Slot text")
	UTextBlock* IngredientsQuntity;
public:
	void SetData(const FCraftingIngredient& IngredientData);
	
	
};
