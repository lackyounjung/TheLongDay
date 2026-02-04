// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "GearItemData.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* OwnerActor = GetOwner();

	if (OwnerActor)
	{
		// 2. OwnerActor에서 UInventoryComponent를 찾습니다.
		// FindComponentByClass 함수는 액터에 부착된 해당 타입의 첫 번째 컴포넌트를 반환합니다.
		InventoryComponent = OwnerActor->FindComponentByClass<UInventoryComponent>();

		// 3. 컴포넌트가 성공적으로 찾아졌는지 확인합니다.
		if (InventoryComponent)
		{
			UE_LOG(LogTemp, Log, TEXT("InventoryComponent를 성공적으로 찾았습니다."));
		}
		else
		{
			// 컴포넌트를 찾지 못했을 경우 경고 로그를 남깁니다.
			UE_LOG(LogTemp, Warning, TEXT("Owner 액터에서 InventoryComponent를 찾을 수 없습니다."));
		}
	}
	m_SocketMap.Add("Torso_Hand", nullptr);
	m_SocketMap.Add("Torso_Inner", nullptr);
	m_SocketMap.Add("Torso_Outer", nullptr);
	m_SocketMap.Add("Weapon", nullptr);
	m_SocketMap.Add("Torso_Head", nullptr);
	m_SocketMap.Add("Torso_Feet", nullptr);

	
	// ...
	
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UEquipmentComponent::EquipItem(FName ItemName)
{
	AActor* OwnerActor = GetOwner();

	

	if (ItemName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem failed: ItemID is None."));
		return false;
	}

	if (ItemName == "Bow")
		EquipItem("Arrow");

	//현재 tmap에 해당아이템이 들어갈 소켓의 이름과 정보 가져오기
	if (m_ItemdataTable)
	{
		// ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
		if (const FGearItemData* MasterRow = m_ItemdataTable->FindRow<FGearItemData>(ItemName, TEXT("Find GearItemData")))
		{
			
			FName EquipSocketName = MasterRow->EquipSocketName;
			FVector EquipLocationOffset = MasterRow->EquipLocationOffset;
			FRotator EquipRotationOffset = MasterRow->EquipRotationOffset;
			TSubclassOf<AActor> EquipClass = MasterRow->ItemActorClass;

			//방식1

			TArray<UStaticMeshComponent*> StaticMeshComponents;
			OwnerActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
			UStaticMeshComponent* TargetComponent = nullptr;
			for (UStaticMeshComponent* Component : StaticMeshComponents)
			{
				if (Component->GetFName() == EquipSocketName)
				{
					TargetComponent = Component;
					break;
				}
			}
			const FGearItemData** ExistingRowPtr = m_SocketMap.Find(EquipSocketName);
			if (ExistingRowPtr != nullptr)
			{
				const FGearItemData* ExistingItemData = *ExistingRowPtr;

				if (ExistingItemData != nullptr) // 이미 아이템이 장착되어 있음
				{
					
					// 안전하게 아이템 ID에 접근
					if (InventoryComponent)
					{
						if (ExistingItemData->ItemID == "Arrow")
						{
							UnEquipItem("Bow");
						}
						else
						{
							UnEquipItem(ExistingItemData->ItemID);
						}
						//InventoryComponent->AddItem(ExistingItemData->ItemID);
					}
				}
				// 4. 새로운 아이템 데이터로 갱신 (Add는 키가 이미 있으면 덮어씁니다.)
				m_SocketMap.Add(EquipSocketName, MasterRow);
			}
			else
			{
				// 키가 존재하지 않음 (BeginPlay에서 미리 추가하지 않은 슬롯이거나, EquipSocketName이 잘못된 경우)
				UE_LOG(LogTemp, Warning, TEXT("EquipItem failed: SocketName %s not found in m_SocketMap."), *EquipSocketName.ToString());
				//
				// 
				// GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("fail equip because point"), false);
				// 키가 없을 경우 새로 추가하면서 장착을 시도할 수도 있습니다.
				m_SocketMap.Add(EquipSocketName, MasterRow);
			}
			
			TargetComponent->SetStaticMesh(MasterRow->StaticMesh);
			TargetComponent->SetRelativeLocationAndRotation(EquipLocationOffset, EquipRotationOffset);
			if (MasterRow->GearType == EGearType::ET_Weapon)
			{
				setCurWeapon(ItemName);
			}
			
			return true;

			


		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
	





bool UEquipmentComponent::UnEquipItem(FName ItemName)
{
	AActor* OwnerActor = GetOwner();

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	OwnerActor->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	UStaticMeshComponent* TargetComponent = nullptr;

	if(ItemName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("AddItem failed: ItemID is None."));
		return false;
	}
	//현재 tmap에 해당아이템이 들어갈 소켓의 이름과 정보 가져오기
	if (m_ItemdataTable)
	{
		// ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
		if (const FGearItemData* MasterRow = m_ItemdataTable->FindRow<FGearItemData>(ItemName, TEXT("Find GearItemData")))
		{
			FName EquipSocketName = MasterRow->EquipSocketName;
			
			for (UStaticMeshComponent* Component : StaticMeshComponents)
			{
				if (Component->GetFName() == EquipSocketName)
				{
					TargetComponent = Component;
					break;
				}
			}


			//만약 소켓자리에 아이템이 없다면 
			if (m_SocketMap.Find(EquipSocketName) == nullptr)
			{
				return false;
			}
			else
			{
				InventoryComponent->AddItem(m_SocketMap[EquipSocketName]->ItemID);
				m_SocketMap[EquipSocketName] = nullptr;
			}

			TargetComponent->SetStaticMesh(NULL);

			// 기본 아이템 데이터 찾음
			//InventoryItemsData.Add(*MasterRow);
			//OnInventoryChanged.Broadcast();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}

const FGearItemData* UEquipmentComponent::EquipitemCheck(FName SocketName)
{
	const FGearItemData* itemdata = m_SocketMap[SocketName];
	return itemdata;
}

void UEquipmentComponent::setCurWeapon(FName WeaponID)
{

	if (m_ItemdataTable)
	{
		// ItemMasterTable의 Row 구조체가 FItem_Data를 상속받거나 FItem_Data와 동일하다고 가정합니다.
		if (const FGearItemData* MasterRow = m_ItemdataTable->FindRow<FGearItemData>(WeaponID, TEXT("Find GearItemData")))
		{
			EWeaponType EquipWeaponName = MasterRow->WeaponType;

			m_curEquipWeapon = EquipWeaponName;
			

			
			m_curWeapon =  MasterRow->ItemActorClass;

		}

	}

	
}

EWeaponType UEquipmentComponent::GetCurWeapon()
{

	return m_curEquipWeapon;
}

TSubclassOf<AActor> UEquipmentComponent::GetCurWeaponClass()
{
	return m_curWeapon;
}

TMap<FString, FName> UEquipmentComponent::GetEquippedDataForSave()
{
	TMap<FString, FName> OutData;
	for (auto& Elem : m_SocketMap)
	{
		if (Elem.Value) // FGearItemData 포인터가 있으면
		{
			OutData.Add(Elem.Key.ToString(), Elem.Value->ItemID);
		}
	}
	return OutData;
	
}

void UEquipmentComponent::LoadEquippedItems(const TMap<FString, FName>& Data)
{
	for (auto& Elem : Data)
	{
		// 기존에 만드신 EquipItem 함수를 재사용합니다.
		// 이 함수가 내부적으로 Mesh 변경과 m_SocketMap 갱신을 수행합니다.
		EquipItem(Elem.Value);
	}
}

