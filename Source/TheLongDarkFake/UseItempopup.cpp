// Fill out your copyright notice in the Description page of Project Settings.


#include "UseItempopup.h"

void UUseItempopup::SetDatainTextAndImage(const FItem_Data& NewItemData)
{
	ItemData = NewItemData;

	if (TextBlock_ItemName)
	{
		FText ftextitemid = FText::FromName(ItemData.ItemID);
		TextBlock_ItemName->SetText(ftextitemid);
	}
	if (TextBlock_Description)
	{
		TextBlock_Description->SetText(ItemData.Description);
	}
	if (Image_Item)
	{
		Image_Item->SetBrushFromTexture(ItemData.IconTexture);
	}
}
