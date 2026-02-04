// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "string.h"
#include "CraftingData.h"
#include "InteractableInterface.h"
#include "CraftingComponent.h"
#include "CraftingBase.generated.h"

UCLASS()
class THELONGDARKFAKE_API ACraftingBase : public AActor , public IInteractableInterface
{
	GENERATED_BODY()

private:
	
	
public:	
	// Sets default values for this actor's properties
	ACraftingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "crafting")
	FName m_RecipeName;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TArray<FCraftingIngredient> m_Ingredient;*/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*void SetRecipeName(FName RecipeName);
	void SetIngredients(const TArray<FCraftingIngredient>* Ingredients);*/
	FName GetRecipeName();
	/*TArray<FCraftingIngredient> GetIngredient();*/
	bool Interact_Implementation(AActor* player) override;
};
