// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerSurvivalStatComponent.h"
#include "Engine/DataTable.h"   
#include "Item_Data.h"
#include "FoodItemData.h"
#include "GearItemData.h"
#include "InventoryComponent.generated.h"

// 델리게이트 선언: 인자가 없는 일반적인 멀티캐스트 델리게이트입니다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddItem, FName, ItemID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	float m_Totalgram{};
	float m_fMaxGram{};
	float m_fCurGram{};
	FItem_Data m_FindItemData{};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FItem_Data> InventoryItemsData{};

	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* ItemMasterTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* GearItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* FoodItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	UPlayerSurvivalStatComponent* PlayerStatComponent;
	

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	bool AddItem(FName ItemID);
	UFUNCTION(BlueprintCallable)
	void RemoveItem(const FItem_Data& ItemData);
	UFUNCTION(BlueprintCallable)
	void UseItem(FItem_Data ItemData);
	UFUNCTION(BlueprintCallable)
	FItem_Data Finditemdata(FName itemId);
	FItem_Data* FindItem(FName ItemID);
	TArray<FItem_Data> GetInventory();
	// 델리게이트 변수 추가 (BlueprintCallable로 Blueprint에서도 접근 가능하게)
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnAddItem OnAddItem;
	void SetInventoryData(const TArray<FItem_Data>& NewData) { InventoryItemsData = NewData; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
