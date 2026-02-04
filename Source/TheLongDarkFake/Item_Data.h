// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Item_Data.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_None        UMETA(DisplayName = "None"),          // 기본값/미지정
    EIT_Consumable  UMETA(DisplayName = "Consumable"),    // 소모품 (음식, 약품 등)
    EIT_Equipment   UMETA(DisplayName = "Equipment"),     // 장비 (옷, 도구, 무기 등)
    EIT_Resource    UMETA(DisplayName = "Resource"),      // 자원/재료 (나무, 돌, 가죽)
    EIT_Tool        UMETA(DisplayName = "Tool"),          // 사용 가능한 도구 (라이터, 손전등 등)
    EIT_KeyItem     UMETA(DisplayName = "Key Item"),      // 열쇠, 문서 등 퀘스트 아이템
    EIT_BuildItem     UMETA(DisplayName = "Build Item"),      //건축아이템
};

USTRUCT(BlueprintType)
struct FItem_Data : public FTableRowBase
{
	GENERATED_BODY()
public:
    // 1. 식별 및 표시
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FName ItemID; // 고유 ID (데이터 테이블 Row Name과 일치 권장)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText DisplayName; // 인벤토리 표시 이름

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    EItemType ItemType = EItemType::EIT_None; // 아이템의 타입

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText Description; // 아이템 설명

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
    //TSoftObjectPtr<UTexture2D> IconTexture; // 아이템 아이콘 (SoftPtr로 메모리 효율화)
    UTexture2D* IconTexture;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
    UStaticMesh* StaticMesh;
    // 2. 기본 속성
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    int32 MaxStackSize = 1; // 최대 스택 가능 개수 (1이면 비스택 가능)

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float Weight = 0.0f; // 아이템 1개당 무게

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    TSubclassOf<AActor> ItemActorClass;
    // ------------------------------------------------------------------
    // 3. 장비 (Equipment) 속성: 장착 위치 보정을 위한 오프셋 추가
    // ------------------------------------------------------------------

    /** * 장착 시 캐릭터 소켓을 기준으로 장비 메시의 상대 위치(Offset)를 보정합니다.
     * 모든 장비는 하나의 소켓(예: hand_r)에 부착되며, 이 값으로 아이템별 잡는 위치를 조정합니다.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "ItemType == EItemType::EIT_Equipment", EditConditionHides))
    FVector EquipLocationOffset = FVector::ZeroVector;

    /** * 장착 시 캐릭터 소켓을 기준으로 장비 메시의 상대 회전(Rotation)을 보정합니다.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "ItemType == EItemType::EIT_Equipment", EditConditionHides))
    FRotator EquipRotationOffset = FRotator::ZeroRotator;

    /** * 장비를 부착할 캐릭터 스켈레탈 메시 소켓의 이름입니다.
     * (예: hand_r_socket, spine_03_socket 등)
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (EditCondition = "ItemType == EItemType::EIT_Equipment", EditConditionHides))
    FName EquipSocketName = "hand_r_socket"; // 기본값 설정


};
