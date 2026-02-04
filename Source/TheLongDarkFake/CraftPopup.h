// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingData.h"
#include <string>
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "IngredientsSlotWidget.h"
#include "CraftPopup.generated.h"


/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UCraftPopup : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	FCraftingRecipe m_Recipe;
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "CraftitemData")
	UTextBlock* RecipeID;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "CraftitemData")
	UTextBlock* Description;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "CraftitemData")
	UTextBlock* CraftingTime;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget),Category = "CraftitemData")
	class UUniformGridPanel* NeadIngredientsGrid; 

	UPROPERTY(EditDefaultsOnly, Category = "CraftPopup")
	TSubclassOf<UIngredientsSlotWidget> CraftPopupSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "CraftPopup")
	TArray<UIngredientsSlotWidget*> IngredientsSlots;

	bool SetDataInPopup(const FCraftingRecipe& RecipeData);

	UFUNCTION(BlueprintCallable, Category = "CraftPopup")
	void SetupCraftSlotLayout(const FCraftingRecipe& RecipeData);

	UFUNCTION(BlueprintCallable)
	FCraftingRecipe GetRecipe();
	
};
