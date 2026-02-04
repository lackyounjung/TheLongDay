// Fill out your copyright notice in the Description page of Project Settings.


#include "IngredientsSlotWidget.h"

void UIngredientsSlotWidget::SetData(const FCraftingIngredient& IngredientData)
{
	
	m_IngredientData = IngredientData;
	FText InId = FText::FromName(IngredientData.ItemID);
	IngredientsTitle->SetText(InId);
	float MaxValue = IngredientData.Quantity;
	
	const FText FormatPattern = NSLOCTEXT("ItemFormat", "WeightUnit", "{0}");
	FText InText = FText::Format(FormatPattern, FText::AsNumber(MaxValue));
	IngredientsQuntity->SetText(InText);
}




