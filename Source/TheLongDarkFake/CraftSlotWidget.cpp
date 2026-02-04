// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftSlotWidget.h"
#include "CraftUIWidget.h"

void UCraftSlotWidget::SetCraftSlotData(const FCraftingRecipe& NewRecipe, bool bNewIsEmpty)
{
	CraftRecipeData = NewRecipe;
	FText InText = FText::FromName(NewRecipe.RecipeID);
	RecipeTitle->SetText(InText);
	UTexture2D* InTexture = NewRecipe.IconTexture;
	RecipeImage->SetBrushFromTexture(InTexture, false);
}

void UCraftSlotWidget::UseCraftSlotItem(const FCraftingRecipe& NewRecip)
{
	CraftUIWidget->SetupCraftPopup(NewRecip);
}



