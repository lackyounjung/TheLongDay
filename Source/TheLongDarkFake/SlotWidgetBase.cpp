// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotWidgetBase.h"
#include "Internationalization/Text.h"
#include "UseItempopup.h"
#include "Engine/Engine.h"
#include "InventoryWidgetBase.h"
#include "CraftUIWidget.h"
#include "CraftSlotWidget.h"

void USlotWidgetBase::SetSlotData(const FItem_Data& NewItemData, bool bNewIsEmpty)
{
	ItemData = NewItemData;
	bIsEmpty = bNewIsEmpty;
	

	if (SlotImage)
	{
		SlotImage->SetBrushFromTexture(ItemData.IconTexture);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("SETSLOTDATAimage "), false);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("SETSLOTDATAfalse "), false);
	}

	if (kgtext)
	{
		float WeightValue = ItemData.Weight;
		const FText FormatPattern = NSLOCTEXT("ItemFormat", "WeightUnit", "{0}kg");
		FText InText = FText::Format(FormatPattern, FText::AsNumber(WeightValue));
		kgtext->SetText(InText);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("SETSLOTDATAtext "), false);
	}
	else
	{
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("SETSLOTDATAfalse "), false);
		}
	}

	
}


void USlotWidgetBase::OnSlotUpdated()
{

}



void USlotWidgetBase::UseSlotItem()
{
	InventoryWidget->SetupItemPopup(ItemData);
}


