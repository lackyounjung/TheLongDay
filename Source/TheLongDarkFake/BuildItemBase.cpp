// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildItemBase.h"

// Sets default values
ABuildItemBase::ABuildItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ABuildItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName ABuildItemBase::GetItemID()
{
	return m_ItemID;
}

EItemType ABuildItemBase::GetItemType()
{
	return m_ItemType;
}

bool ABuildItemBase::Interact_Implementation(AActor* player)
{
	return true;
}

void ABuildItemBase::OnPopupConfirmed_Implementation()
{
	
}
