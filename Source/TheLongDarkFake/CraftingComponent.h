// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.h"
#include "Item_Data.h"
#include "LoadingWidget.h"
#include "CraftingData.h"
#include "CraftingComponent.generated.h"

// 델리게이트 선언: 인자가 없는 일반적인 멀티캐스트 델리게이트입니다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCraftInventoryChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UCraftingComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	TArray<FCraftingRecipe> m_Recipes{};
	UPROPERTY(VisibleAnywhere)
	TArray<FCraftingIngredient> m_arrayIngrediant{};

	FTimerHandle CraftingTimeHandle;

	float TotalcraftingTime = 0;
	float CurrentCraftingTime = 0;
	const float TimeUpdateinterval = 0.1f;

	FName m_PendingResultItemID{};
	int m_PendingResultQuantity{};







	
public:	
	// Sets default values for this component's properties
	UCraftingComponent();

	UFUNCTION()
	bool FindCraftingRecipe(FName Recipe);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULoadingWidget> LoadingWidgetClass;

	UPROPERTY()
	class ULoadingWidget* LoadingWidgetInstance;

	UFUNCTION(BlueprintCallable)
	bool Crafting(FCraftingRecipe Recipe);

	void LoadingTime();

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* CraftingDataTabl;

	// 델리게이트 변수 추가 (BlueprintCallable로 Blueprint에서도 접근 가능하게)
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnCraftInventoryChanged FOnCraftInventoryChange;
	void SetRecipeData(const TArray<FCraftingRecipe>& NewData) { m_Recipes = NewData; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	TArray<FCraftingRecipe> GetRecipes();

		
};
