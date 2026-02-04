// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"        // UDataTable 사용을 위해 필요
#include "SetEfectInterface.h"       // ISetEfectInterface 사용을 위해 필요
#include "StatusEffectData.h"        // FEffectData 및 FSingleEffectEntry 구조체 사용을 위해 필요
#include "DotEffectClass.h"
#include "MovementEffectClass.h"
#include "SpecialAbilityEffectClass.h"
#include "MySaveGame.h"
//#include "StatusEffectData.h"
#include "PlayerEffectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplied, FName, EffectID);

// 런타임에 적용 중인 효과 인스턴스를 관리하기 위한 구조체
USTRUCT()
struct FActiveEffect
{
    GENERATED_BODY()

    // 런타임에 생성된 실제 효과 객체 (ISetEfectInterface 구현)
    UPROPERTY(VisibleAnywhere)
    TScriptInterface<ISetEfectInterface> EffectInstance;

    // 이 효과를 정의한 데이터 테이블의 RowName
    UPROPERTY(VisibleAnywhere)
    FName EffectID;

    // 효과 잔여 시간 (0.0f이면 영구, Tick마다 감소)
    UPROPERTY(VisibleAnywhere)
    float RemainingTime;

    UPROPERTY(VisibleAnywhere)
    TArray<FSingleEffectEntry> Entrys;
    UPROPERTY(VisibleAnywhere)
    TArray<TScriptInterface<ISetEfectInterface>> EntrysEffectInstance;

    // 원본 FEffectData의 복사본 (옵션)
    // UPROPERTY()
    // FEffectData EffectData; // 너무 큰 데이터는 제외하고 필요한 값만 저장하는 것이 좋습니다.
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UPlayerEffectComponent : public UActorComponent
{
	GENERATED_BODY()
private:
    // 1. 데이터 테이블 참조 변수 (필수)
    UPROPERTY(EditDefaultsOnly, Category = "Data")
    class UDataTable* StatusEffectsDataTable;
    

    // 2. 현재 적용 중인 효과 인스턴스 맵 (필수)
    UPROPERTY(VisibleAnywhere, Category = "Status")
    TMap<FName, FActiveEffect> ActiveEffects;

    // 3. 효과 지속 시간 관리를 위한 타이머 핸들 (필수)
    FTimerHandle DurationTickHandle;

    // 4. 시간 관리를 위한 내부 함수 (필수)
    void HandleDurationTick();
    
public:	
	// Sets default values for this component's properties
	UPlayerEffectComponent();
    // 5. 핵심: 외부에서 디버프/버프를 적용할 함수 (ApplyEffect)
    UFUNCTION(BlueprintCallable, Category = "Status")
    bool ApplyEffect(FName EffectID);

    // 6. 핵심: 디버프/버프를 즉시 제거할 함수 (RemoveEffect)
    UFUNCTION(BlueprintCallable, Category = "Status")
    bool RemoveEffect(FName EffectID);
	UFUNCTION(BlueprintCallable, Category = "Status | Debug")
	void DebugPrintEffectJson(FName EffectID);
    UPROPERTY(BlueprintAssignable, Category = "Status")
    FOnStatusChanged OnStatusChanged;
    UPROPERTY(BlueprintAssignable, Category = "Status")
    FOnEffectApplied OnEffectApplied;

    const TMap<FName, FActiveEffect>& GetActiveEffects() const { return ActiveEffects; }
    UDataTable* GetStatusDataTable() const { return StatusEffectsDataTable; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    void GetEffectsForSave(TArray<FSavedEffectData>& OutEffects)
    {
        for (auto& Pair : ActiveEffects)
        {
            FSavedEffectData Data;
            Data.EffectID = Pair.Key;
            Data.RemainingTime = Pair.Value.RemainingTime;
            OutEffects.Add(Data);
        }
    }

    void LoadEffects(const TArray<FSavedEffectData>& InEffects)
    {
        for (const FSavedEffectData& Data : InEffects)
        {
            if (ApplyEffect(Data.EffectID))
            {
                // ApplyEffect로 생성된 인스턴스의 시간을 저장된 시간으로 덮어씀
                if (ActiveEffects.Contains(Data.EffectID))
                {
                    ActiveEffects[Data.EffectID].RemainingTime = Data.RemainingTime;
                }
            }
        }
    }

		
};
