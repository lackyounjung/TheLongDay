// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Data.h"
#include "string.h"
#include "Engine/DataTable.h"
#include "InventoryComponent.h"
#include "InteractableInterface.h"
#include "SetPopupInterface.h"
#include "BuildItemBase.generated.h"

UCLASS()
class THELONGDARKFAKE_API ABuildItemBase : public AActor, public IInteractableInterface, public ISetPopupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ItemMesh;

	// **이것이 핵심:** 상호작용 로직이 아닌, 외부(플레이어)가 읽을 수 있는 데이터만 제공
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName m_ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType m_ItemType{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	UDataTable* m_ItemdataTable{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	FName GetItemID();
	EItemType GetItemType();
	virtual bool Interact_Implementation(AActor* player) override;
	virtual void OnPopupConfirmed_Implementation() override;

};
