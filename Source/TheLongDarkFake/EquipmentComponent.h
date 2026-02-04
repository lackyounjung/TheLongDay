// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GearItemData.h"
#include "ItemBase.h"
#include "InventoryComponent.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()
	
	TMap<FName ,const FGearItemData*> m_SocketMap{};
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	EWeaponType m_curEquipWeapon{};

	UPROPERTY()
	TSubclassOf<AActor> m_curWeapon{};

	UPROPERTY()
	TObjectPtr<AActor> CurrentWeaponActor;

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UDataTable* m_ItemdataTable{};

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CharacterMesh;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	bool EquipItem(FName ItemName);
	
	UFUNCTION(BlueprintCallable)
	bool UnEquipItem(FName ItemName);
	
	const FGearItemData* EquipitemCheck(FName SocketName);
	UFUNCTION(BlueprintCallable)
	void  setCurWeapon(FName WeaponID);
	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurWeapon();
	
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetCurWeaponClass();

	// 현재 장착된 맵 데이터를 반환 (저장용)
	TMap<FString, FName> GetEquippedDataForSave();

	// 데이터를 받아서 실제 장착 처리 (로드용)
	void LoadEquippedItems(const TMap<FString, FName>& Data);
};
