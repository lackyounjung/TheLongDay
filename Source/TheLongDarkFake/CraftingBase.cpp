// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingBase.h"

// Sets default values
ACraftingBase::ACraftingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

	// ItemMesh를 이 액터의 루트 컴포넌트로 설정합니다.
	// 모든 액터는 하나의 루트 컴포넌트를 가져야 안정적입니다.
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ACraftingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACraftingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACraftingBase::SetRecipeName(FName RecipeName)
//{
//	m_RecipeName = RecipeName;
//}
//
//void ACraftingBase::SetIngredients(const TArray<FCraftingIngredient>* Ingredients)
//{
//	m_Ingredient = *Ingredients;
//}

FName ACraftingBase::GetRecipeName()
{
	return m_RecipeName;
}

//TArray<FCraftingIngredient> ACraftingBase::GetIngredient()
//{
//	return m_Ingredient;
//}

bool ACraftingBase::Interact_Implementation(AActor* player)
{
	UCraftingComponent* Craftingcom = player->FindComponentByClass<UCraftingComponent>();

	if (Craftingcom)
	{
		if (Craftingcom->FindCraftingRecipe(m_RecipeName))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses interact"), false);
			Destroy();
			return true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("false interact"), false);
			
			
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("Craftingfalse"), false);
	}
	return false;
}

