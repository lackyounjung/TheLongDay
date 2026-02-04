// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "string.h"
#include "InteractableInterface.h"
#include "DropItemInterface.h"
#include "InventoryComponent.h"
#include "Engine/DataTable.h"
#include "ResourceBase.generated.h"

UCLASS()
class THELONGDARKFAKE_API AResourceBase : public AActor, public IInteractableInterface, public IDropItemInterface
{
	GENERATED_BODY()
private:
	// 반복 타이머를 제어하기 위한 핸들
	FTimerHandle DepletionTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class ULoadingWidget> LoadingWidgetClass;

	UPROPERTY()
	class ULoadingWidget* LoadingWidgetInstance;

	

	// 총 채집에 필요한 시간 (5.0초)
	float TotalGatheringTime = 5.0f;

	// 현재 경과 시간 (타이머마다 증가)
	float CurrentGatheringTime = -1.0f;

	// 타이머 반복 간격 (예: 0.1초마다 업데이트)
	const float TimerUpdateInterval = 0.1f;



	
public:	
	// Sets default values for this actor's properties
	AResourceBase();

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* ResourceDataTabl;
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* dropitemDataTabl;
	UPROPERTY(VisibleAnywhere, Category = "Data" )
	FName m_DropItemName{};
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TSubclassOf<AActor> DropItemActorClass;
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TArray<FName> m_ItemsName{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 동적 머티리얼 인스턴스 포인터 (파괴도 막아야 하므로 UPROPERTY 필요)
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ResourceMesh;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	class UDataTable* ResourceDataTable;*/

	// **이것이 핵심:** 상호작용 로직이 아닌, 외부(플레이어)가 읽을 수 있는 데이터만 제공
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FName m_ResourceID;

	TWeakObjectPtr<AActor> InstigatorActor;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType m_ResourceType{};*/
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FName GetResourceName();
	bool Interact_Implementation(AActor* player) override;
	bool DropItem_Implementation(AActor* player) override;
	
	void UpdateDissolveEffect();

};
