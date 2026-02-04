// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "PlayerStat.h"
#include "PlayerSurvivalStatComponent.generated.h"

// DT_Player_Stats의 행과 일치해야 하는 마스터 데이터 구조체
USTRUCT(BlueprintType)
struct FStatData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StatID; // Stat의 고유 ID (예: Hunger, Cold)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DecreaseRatePerSec = 0.0f; // 초당 감소율 (2단계 핵심 값)

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalThreshold = 20.0f; // 위험 임계점

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FName DebuffEffectID;

	
};

// 플레이어의 현재 스탯 상태를 런타임에 저장할 구조체
USTRUCT(BlueprintType)
struct FCurrentStat
{
	GENERATED_BODY()
public:
	// 마스터 데이터 참조
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FPlayerStat MasterData;

	// 현재 값 (UI 바인딩 및 저장/로드 대상)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stats")
	float CurrentValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDebuffActive = false;
};


UCLASS( ClassGroup=(Survival), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UPlayerSurvivalStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	// **에디터에서 DT_Player_Stats 에셋을 직접 할당받을 필드**
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* PlayerStatsDataTable;

	// 현재 플레이어의 모든 생존 스탯 목록 (런타임 데이터)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	TArray<FCurrentStat> SurvivalStats;

	// 스탯 감소 로직을 위한 타이머 핸들
	FTimerHandle StatDecreaseTimerHandle;

	/** * @brief 데이터 테이블을 읽어와 스탯 배열을 초기화하고 타이머를 시작합니다.
	 */
	void InitializeStats();

	/** * @brief 타이머에 의해 1초마다 호출되어 스탯을 감소시키는 핵심 로직 (2단계 목표)
	 */
	void DecreaseStats();

public:	
	// **[UI 바인딩 및 아이템 사용을 위한 헬퍼 함수]**

	// 특정 스탯의 현재 값을 가져옵니다. (UI Progress Bar Current)
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetStatCurrentValue(FName StatID) const;

	// 특정 스탯의 최대 값을 가져옵니다. (UI Progress Bar Max)
	UFUNCTION(BlueprintPure, Category = "Stats")
	float GetStatMaxValue(FName StatID) const;

	// 음식 섭취 등으로 스탯을 변화시킵니다.
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeStatValue(FName StatID, float Amount);

	// 음식 섭취 등으로 감소 스탯을 변화시킵니다.
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ChangeDecreaseStatValue(FName StatID, float Amount);
	// Sets default values for this component's properties
	UPlayerSurvivalStatComponent();

	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void GetStatsForSave(TMap<FName, float>& OutStats)
	{
		for (const FCurrentStat& Stat : SurvivalStats)
		{
			OutStats.Add(Stat.MasterData.StatID, Stat.CurrentValue);
		}
	}

	void LoadStats(const TMap<FName, float>& InStats)
	{
		for (FCurrentStat& Stat : SurvivalStats)
		{
			if (InStats.Contains(Stat.MasterData.StatID))
			{
				Stat.CurrentValue = InStats[Stat.MasterData.StatID];
			}
		}
	}

		
};
