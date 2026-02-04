// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingComponent.h"
#include "CraftSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "CraftPopup.h"
#include "CraftUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UCraftUIWidget : public UUserWidget
{
	GENERATED_BODY()
	int32 m_craftslotnum = 40;
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* CraftSlotGrid;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "CraftInventory")
	UCraftPopup* WBP_CraftingPopup;

	UPROPERTY(BlueprintReadWrite, Category = "CraftInventory")
	TWeakObjectPtr<UCraftingComponent> OwnerCraftingComponent;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget), Category = "CraftInventory")
	TArray<UCraftSlotWidget*> CraftSlots;

	UPROPERTY(EditDefaultsOnly, Category = "CraftInventory")
	TSubclassOf<UCraftSlotWidget> CraftSlotWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "CraftInventory")
	void RefreshCraftUI();
    
	UFUNCTION(BlueprintCallable, Category = "CraftInventory")
	void SetupCraftSlotLayout();

	UFUNCTION(BlueprintCallable, Category = "CraftInventory")
	void SetupCraftPopup(const FCraftingRecipe& newRecipe );
};
