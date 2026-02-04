// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "TheLongDarkPlayer.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // UStaticMeshComponent를 생성합니다.
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));

    // ItemMesh를 이 액터의 루트 컴포넌트로 설정합니다.
    // 모든 액터는 하나의 루트 컴포넌트를 가져야 안정적입니다.
    RootComponent = ItemMesh;

    // 필요하다면 여기서 물리 설정 등을 할 수 있습니다.
    //ItemMesh->SetSimulatePhysics(false); // 예: 물리 시뮬레이션 비활성화
    //ItemMesh->SetCollisionProfileName(TEXT("OverlapAll")); // 예: 기본 충돌 설정
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName AItemBase::GetItemID()
{
	return m_ItemID;
}

EItemType AItemBase::GetItemType()
{
	return m_ItemType;
}

bool AItemBase::Interact_Implementation(AActor* player)
{
    // 1. 인벤토리 컴포넌트 찾기 (Instigator가 플레이어 캐릭터라고 가정)
    UInventoryComponent* Inventory = player->FindComponentByClass<UInventoryComponent>();

    if (Inventory)
    {
        // 2. 인벤토리 컴포넌트에 아이템 추가 요청
        
        

        

        if (Inventory->AddItem(m_ItemID))
        {
            ATheLongDarkPlayer* pPlayer = Cast<ATheLongDarkPlayer>(player);
            if (!pPlayer) return false;

            //pPlayer->SetInteractionwithitem(true);
            // 3. 획득 성공 시 월드에서 자신을 파괴
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsses interact"),false);
            Destroy();
            return true;
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("false"), false);
        }
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("inventoryfalse"), false);
    }
    return false;
}

