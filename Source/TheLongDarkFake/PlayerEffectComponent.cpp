// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerEffectComponent.h"
#include "Engine/DataTable.h"

void UPlayerEffectComponent::HandleDurationTick()
{
    // 제거할 효과 ID 목록
    TArray<FName> EffectsToRemove;

    // TMap을 순회하며 시간 감소
    for (auto& Pair : ActiveEffects)
    {
        FActiveEffect& Active = Pair.Value;

        // DurationSeconds가 0.0f이면 영구 효과이므로 건너뜁니다.
        if (Active.RemainingTime > 0.0f)
        {
            // 틱은 1초마다 호출된다고 가정
            Active.RemainingTime -= 1.0f;

            if (Active.RemainingTime <= 0.0f)
            {
                EffectsToRemove.Add(Pair.Key);
            }
        }
    }

    // 시간이 다 된 효과들을 제거
    for (FName EffectID : EffectsToRemove)
    {
        // RemoveEffect를 호출하여 CleanUp 로직까지 처리
        RemoveEffect(EffectID);
    }

    // 모든 효과가 제거되면 DurationTick 타이머도 해제
    if (ActiveEffects.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(DurationTickHandle);
    }
}



// Sets default values for this component's properties
UPlayerEffectComponent::UPlayerEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UPlayerEffectComponent::ApplyEffect(FName EffectID)
{
    if (!StatusEffectsDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ApplyEffect Failed: StatusEffectsDataTable is null!"));
        return false;
    }

    // 1. 데이터 테이블에서 FEffectData 구조체 찾기
    const FEffectData* EffectData = StatusEffectsDataTable->FindRow<FEffectData>(EffectID, TEXT("Find Status Effect Data"));
    if (!EffectData)
    {
        UE_LOG(LogTemp, Warning, TEXT("ApplyEffect Warning: EffectID '%s' not found in data table."), *EffectID.ToString());
        return false;
    }

    // 2. 이미 적용 중인 효과인지 확인 (간단하게 구현)
    if (ActiveEffects.Contains(EffectID))
    {
        UE_LOG(LogTemp, Log, TEXT("Effect '%s' is already active. Refreshing duration logic skipped for now..."), *EffectID.ToString());
        return false;
    }

    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter) return false;

    //FActiveEffect Active;

    FActiveEffect& NewActive = ActiveEffects.FindOrAdd(EffectID);
    NewActive.EffectID = EffectID;
    NewActive.RemainingTime = EffectData->DurationSeconds;
    NewActive.EntrysEffectInstance.Empty(); // 혹시 기존 데이터가 있다면 비워줌

    // 3. EffectEntries 배열을 순회하며 개별 효과 인스턴스 생성 및 주입
    for (const FSingleEffectEntry& Entry : EffectData->EffectEntries)
    {
        if (!Entry.EffectClass) continue;

        // UObject 인스턴스 생성
        UObject* NewEffect = NewObject<UObject>(this, Entry.EffectClass);
        ISetEfectInterface* EffectInterface = Cast<ISetEfectInterface>(NewEffect);
        
        if (EffectInterface)
        {
            if (UDotEffectClass* DotEffect = Cast<UDotEffectClass>(NewEffect))
                DotEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);
            else if (UMovementEffectClass* MoveEffect = Cast<UMovementEffectClass>(NewEffect))
                MoveEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);
            else if (USpecialAbilityEffectClass* SpecialEffect = Cast<USpecialAbilityEffectClass>(NewEffect))
                SpecialEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);

            EffectInterface->Execute_ExecuteEffect(NewEffect);

            
            TScriptInterface<ISetEfectInterface> InstanceWrapper;
            InstanceWrapper.SetInterface(EffectInterface);
            InstanceWrapper.SetObject(NewEffect);

            NewActive.EntrysEffectInstance.Add(InstanceWrapper);
        }
        //if (EffectInterface)
        //{
        //    // 4-1. UDotEffectClass 인스턴스인지 확인하고 데이터 주입
        //    if (UDotEffectClass* DotEffect = Cast<UDotEffectClass>(NewEffect))
        //    {
        //        // JSON 문자열을 전달하여 내부적으로 파싱하도록 호출
        //        DotEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);
        //        // 5. 효과 실행 명령 (타이머 시작)
        //        EffectInterface->Execute_ExecuteEffect(NewEffect);
        //        // 6. ActiveEffects 맵에 등록 (관리 시작)
        //        /*FActiveEffect Active;*/
        //        Active.EffectID = EffectID;
        //        Active.RemainingTime = EffectData->DurationSeconds;
        //        // Active.EffectData = *EffectData; // 대용량 데이터 복사 생략
        //        Active.EffectInstance.SetInterface(EffectInterface);
        //        Active.EffectInstance.SetObject(NewEffect);
        //        Active.EntrysEffectInstance.Add(Active.EffectInstance);
        //        
        //        
        //        ActiveEffects.Add(EffectID, Active);
        //        UE_LOG(LogTemp, Log, TEXT("DOT Effect from '%s' successfully created and started."), *EffectID.ToString());
        //    }
        //    // 4-2. UMovementEffectClass 인스턴스인지 확인하고 데이터 주입
        //    if (UMovementEffectClass* MoveEffect = Cast<UMovementEffectClass>(NewEffect))
        //    {
        //        MoveEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);
        //        // 5. 효과 실행 명령 (타이머 시작)
        //        EffectInterface->Execute_ExecuteEffect(NewEffect);
        //        // 6. ActiveEffects 맵에 등록 (관리 시작)
        //        /*FActiveEffect Active;*/
        //        Active.EffectID = EffectID;
        //        Active.RemainingTime = EffectData->DurationSeconds;
        //        // Active.EffectData = *EffectData; // 대용량 데이터 복사 생략
        //        Active.EffectInstance.SetInterface(EffectInterface);
        //        Active.EffectInstance.SetObject(NewEffect);
        //        Active.EntrysEffectInstance.Add(Active.EffectInstance);
        //        
        //        ActiveEffects.Add(EffectID, Active);
        //        UE_LOG(LogTemp, Log, TEXT("Movement Effect from '%s' successfully created and started."), *EffectID.ToString());
        //    }
        //    // 4-3. USpecialAbilityEffectClass 인스턴스인지 확인하고 데이터 주입
        //    if (USpecialAbilityEffectClass* SpecialEffect = Cast<USpecialAbilityEffectClass>(NewEffect))
        //    {
        //        SpecialEffect->InitializeEffectData(OwnerCharacter, Entry.EffectParametersJson);
        //        // 5. 효과 실행 명령 (타이머 시작)
        //        EffectInterface->Execute_ExecuteEffect(NewEffect);
        //        // 6. ActiveEffects 맵에 등록 (관리 시작)
        //        /*FActiveEffect Active;*/
        //        Active.EffectID = EffectID;
        //        Active.RemainingTime = EffectData->DurationSeconds;
        //        // Active.EffectData = *EffectData; // 대용량 데이터 복사 생략
        //        Active.EffectInstance.SetInterface(EffectInterface);
        //        Active.EffectInstance.SetObject(NewEffect);
        //        Active.EntrysEffectInstance.Add(Active.EffectInstance);
        //        
        //        ActiveEffects.Add(EffectID, Active);
        //        UE_LOG(LogTemp, Log, TEXT("special Effect from '%s' successfully created and started."), *EffectID.ToString());
        //    }
        //    /*else
        //    {
        //        UE_LOG(LogTemp, Warning, TEXT("Effect Class '%s' is not recognized or not UDotEffectClass. Initialization skipped."), *Entry.EffectClass->GetName());
        //    }*/
        //}


    }
    if (!GetWorld()->GetTimerManager().IsTimerActive(DurationTickHandle))
    {
        // 1.0초 주기로 HandleDurationTick을 반복 호출하도록 설정
        GetWorld()->GetTimerManager().SetTimer(
            DurationTickHandle,
            this,
            &UPlayerEffectComponent::HandleDurationTick,
            1.0f, // 1초 간격으로 체크
            true  // 반복
        );
        // PrimaryComponentTick.bCanEverTick = true; (이미 생성자에서 설정됨)
    }

    if (OnStatusChanged.IsBound())
    {
        OnStatusChanged.Broadcast();
    }

    if (OnEffectApplied.IsBound())
    {
        OnEffectApplied.Broadcast(EffectID);
    }

    return true;
    
}

bool UPlayerEffectComponent::RemoveEffect(FName EffectID)
{
    if (!ActiveEffects.Contains(EffectID)) return false;
    // 1. 적용 중인 효과 인스턴스를 가져옵니다.
    FActiveEffect& ActiveEffect = ActiveEffects[EffectID];

    

    TArray<TScriptInterface<ISetEfectInterface>> interfaceentrys = ActiveEffect.EntrysEffectInstance;
    
    

    for (TScriptInterface<ISetEfectInterface>& ininterface : interfaceentrys)
    {
        ISetEfectInterface::Execute_CleanUpEffect(ininterface.GetObject());
    }
    

    //// 2. ISetEfectInterface의 CleanUpEffect를 호출하여 타이머를 중지시키거나 상태를 복구합니다.
    //if (ActiveEffect.EffectInstance.GetInterface())
    //{
    //    ISetEfectInterface::Execute_CleanUpEffect(ActiveEffect.EffectInstance.GetObject());
    //}

    // 3. ActiveEffects 맵에서 제거합니다.
    ActiveEffects.Remove(EffectID);

    UE_LOG(LogTemp, Log, TEXT("Effect '%s' successfully removed and cleaned up."), *EffectID.ToString());

    // **추가:** 만약 ActiveEffects 맵이 비었고 DurationTickHandle이 실행 중이라면 타이머를 해제할 수 있습니다.
    if (ActiveEffects.Num() == 0 && GetWorld()->GetTimerManager().IsTimerActive(DurationTickHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(DurationTickHandle);
        PrimaryComponentTick.bCanEverTick = false; // DurationTick이 없으면 굳이 Tick이 필요 없을 수 있음
    }

    if (OnStatusChanged.IsBound())
    {
        OnStatusChanged.Broadcast();
    }

    return true;
}

void UPlayerEffectComponent::DebugPrintEffectJson(FName EffectID)
{
    if (!StatusEffectsDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("DebugPrintEffectJson Failed: StatusEffectsDataTable is null!"));
        return;
    }

    // 1. 데이터 테이블에서 FEffectData 구조체 찾기
    const FEffectData* EffectData = StatusEffectsDataTable->FindRow<FEffectData>(EffectID, TEXT("Debug Get Status Effect Data"));

    if (!EffectData)
    {
        UE_LOG(LogTemp, Warning, TEXT("DebugPrintEffectJson Warning: EffectID '%s' not found in data table."), *EffectID.ToString());
        return;
    }

    // 2. EffectEntries 배열이 비어있지 않은지 확인
    if (EffectData->EffectEntries.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("DebugPrintEffectJson Warning: EffectID '%s' has no entries in EffectEntries array."), *EffectID.ToString());
        return;
    }

    // 3. 첫 번째 효과 엔트리의 JSON 문자열 출력
    const FSingleEffectEntry& FirstEntry = EffectData->EffectEntries[0];

    UE_LOG(LogTemp, Log, TEXT(" Effect ID: %s"), *EffectID.ToString());
    UE_LOG(LogTemp, Log, TEXT(" Effect Class: %s"), *FirstEntry.EffectClass->GetName());
    UE_LOG(LogTemp, Log, TEXT(" JSON Params: %s"), *FirstEntry.EffectParametersJson);

    // 참고: 여기서 DotEffectClass의 InitializeEffectData를 직접 호출하여 파싱 테스트도 가능합니다.
    // UDotEffectClass* TestDot = NewObject<UDotEffectClass>(this);
    // TestDot->InitializeEffectData(Cast<ACharacter>(GetOwner()), FirstEntry.EffectParametersJson);
}


// Called when the game starts
void UPlayerEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

