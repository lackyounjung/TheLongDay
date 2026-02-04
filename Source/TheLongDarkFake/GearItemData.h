// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item_Data.h"
#include "GearItemData.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EGearType : uint8
{
	ET_Tool     UMETA(DisplayName = "Tool"),       // 도끼, 칼, 횃불 등
	ET_Clothing UMETA(DisplayName = "Clothing"),    // 코트, 바지, 부츠 등
	ET_Weapon   UMETA(DisplayName = "Weapon"),      // 활, 총 (옵션)
	ET_Container UMETA(DisplayName = "Container"), // 무게 제한 증가
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ET_None    UMETA(DisplayName = "None"), 
	ET_Axe     UMETA(DisplayName = "Axe"),       
	ET_Gun     UMETA(DisplayName = "Gun"),   
	ET_Arrow   UMETA(DisplayName = "Arrow"),      
	
};

UENUM(BlueprintType)
enum class EClothingLayer : uint8
{
	EL_None        UMETA(DisplayName = "None"),
	EL_Head     UMETA(DisplayName = "Head"),
	EL_Torso_Inner UMETA(DisplayName = "Torso_Inner"),
	EL_Torso_Outer UMETA(DisplayName = "Torso_Outer"),
	EL_Hands    UMETA(DisplayName = "Hands"),
	EL_Feet     UMETA(DisplayName = "Feet"),
	// ... 기타 부위
};

USTRUCT(BlueprintType)
struct FGearItemData : public FItem_Data
{
	GENERATED_BODY()
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	EGearType GearType;//장비의 종류를 분류합니다. (예: Clothing, Tool, Weapon)
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	float MaxDurability; //아이템의 최대 내구도. 사용 시 이 값이 감소합니다.
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	EClothingLayer ClothingLayer; //장비가 옷일 경우 착용되는 부위 (예: Inner, Outer, Head).

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	EWeaponType WeaponType; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	FName StatModifierID; //장비 착용/소지 시 플레이어 스탯에 영구적 또는 지속적으로 적용되는 효과의 ID. (예: Warm_Coat_Effect)
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	float ConditionLossRate; //아이템 사용 시 또는 일정 시간 경과 시 내구도가 감소하는 비율/정도.
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear Stats")
	float ActionDamage; //도구/무기일 경우 자원 채집이나 사냥 시 적용되는 기본 피해량

};
