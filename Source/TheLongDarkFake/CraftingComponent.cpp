// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingComponent.h"
#include "CraftingData.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCraftingComponent::UCraftingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UCraftingComponent::FindCraftingRecipe(FName Recipe)
{
	if (CraftingDataTabl)
	{
		if (const FCraftingRecipe* MasterRow = CraftingDataTabl->FindRow<FCraftingRecipe>(Recipe, TEXT("Find craftingItem Data")))
		{
			// 기본 아이템 데이터 찾음
			m_Recipes.Add(*MasterRow);
			m_arrayIngrediant = MasterRow->Ingredients;
			FOnCraftInventoryChange.Broadcast();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsess find"), false);
			return true;

		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("false find"), false);
			return false;
		}
			
	}
	else
		return false;

}



bool UCraftingComponent::Crafting(FCraftingRecipe Recipe)
{
	AActor* player = GetOwner();

	if (!player)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("player false"), false);
		return false;
	}

	UInventoryComponent* Inventory = player->FindComponentByClass<UInventoryComponent>();

	if (!Inventory)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("Inventory false"), false);
		return false;
	}

	int ingrediantnum = m_arrayIngrediant.Num();
	for (int i = 0; i < ingrediantnum; i++)
	{
		FName ingrediantname = m_arrayIngrediant[i].ItemID;
		int ingreidantelementnum = m_arrayIngrediant[i].Quantity;
		for (int j = 0; j < ingreidantelementnum; j++)
		{
			FItem_Data* finditem = Inventory->FindItem(ingrediantname);
			if (finditem->ItemID != m_arrayIngrediant[i].ItemID)
			{
				FString debugmessage = FString::Printf(TEXT("Failed to find item : %s"), *(ingrediantname.ToString()));
				//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, debugmessage, false);
				return false;
			}
			else
			{
				Inventory->RemoveItem(*finditem);
				FString debugmessage = FString::Printf(TEXT("sucsess to find item : %s"), *(finditem->ItemID.ToString()));
				//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, debugmessage, false);
				//GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, TEXT("remove ingrediant"), false);
			}
			
		}
	}

	if (LoadingWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
		if (PC)
		{
			LoadingWidgetInstance = CreateWidget<ULoadingWidget>(PC, LoadingWidgetClass);
			if (LoadingWidgetInstance)
			{
				LoadingWidgetInstance->AddToViewport(11);
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("failed!"), false);
			}
		}
	}
	

	




	m_PendingResultItemID = Recipe.ResultItemID;
	m_PendingResultQuantity = Recipe.ResultQuantity;
	TotalcraftingTime = Recipe.CraftingTimeSeconds;

	FTimerDelegate TimeDelegate;
	TimeDelegate.BindUObject(this, &UCraftingComponent::LoadingTime);


	if (player && player->GetWorld())
	{
		FTimerManager& TimerManager = player->GetWorld()->GetTimerManager();

		TimerManager.SetTimer(CraftingTimeHandle, TimeDelegate, TimeUpdateinterval, true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Yellow, TEXT("Crafting Timer Started!"), false);
		return true;
	}

	

	return false;
}

void UCraftingComponent::LoadingTime()
{
	AActor* player = GetOwner();
	if (!player) return;

	CurrentCraftingTime += TimeUpdateinterval;
	float progress = FMath::Clamp(CurrentCraftingTime / TotalcraftingTime, 0.0f, 1.0f);


	if (LoadingWidgetInstance)
	{

		if (LoadingWidgetInstance->CallBlueprintFunction(progress))
		{
			//FString DebugMessage = FString::Printf(TEXT("progress: %f"), Progress);

			//// 화면에 출력
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, DebugMessage, false);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("progress!"), false);
		}

	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("faildwidget!"), false);
	}


	if (progress >= 1.0f)
	{
		// 타이머 중지 (반복을 멈춥니다)
		player->GetWorld()->GetTimerManager().ClearTimer(CraftingTimeHandle);

		// 액터 유효성 재확인 (안전 장치)
		if (!IsValid(this)) return;

		LoadingWidgetInstance->RemoveFromParent();
		LoadingWidgetInstance = nullptr;

		CurrentCraftingTime = 0;

		UInventoryComponent* Inventory = player->FindComponentByClass<UInventoryComponent>();

		if (!Inventory)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("Inventory false"), false);
			return;
		}

		for (int i = 0; i < m_PendingResultQuantity; i++)
		{
			Inventory->AddItem(m_PendingResultItemID);
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("craftingtimer end"), false);
	}


	
}


// Called when the game starts
void UCraftingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCraftingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FCraftingRecipe> UCraftingComponent::GetRecipes()
{
	return m_Recipes;
}

