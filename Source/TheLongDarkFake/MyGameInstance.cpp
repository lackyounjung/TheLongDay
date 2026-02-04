// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "CraftingComponent.h"
#include "EquipmentComponent.h"
#include "PlayerSurvivalStatComponent.h"
#include "PlayerEffectComponent.h"
#include "QuestComponent.h"

void UMyGameInstance::SaveGame()
{
    // 1. 세이브 객체 생성
    UMySaveGame* SaveInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    if (!SaveInstance) return;

    // 2. 현재 월드의 플레이어 캐릭터 가져오기
    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerChar)
    {
        // 위치/회전 저장
        SaveInstance->PlayerLocation = PlayerChar->GetActorLocation();
        SaveInstance->PlayerRotation = PlayerChar->GetActorRotation();

        // 1. 스탯 저장
        if (UPlayerSurvivalStatComponent* StatComp = PlayerChar->FindComponentByClass<UPlayerSurvivalStatComponent>())
        {
            StatComp->GetStatsForSave(SaveInstance->SavedStats);
        }

        // 2. 버프/디버프 저장
        if (UPlayerEffectComponent* EffectComp = PlayerChar->FindComponentByClass<UPlayerEffectComponent>())
        {
            EffectComp->GetEffectsForSave(SaveInstance->SavedActiveEffects);
        }

        // 인벤토리 데이터 저장 (컴포넌트 접근)
        UInventoryComponent* InvComp = PlayerChar->FindComponentByClass<UInventoryComponent>();
        if (InvComp)
        {
            SaveInstance->SavedInventoryItems = InvComp->GetInventory();
        }

        // 크래프팅 데이터 저장
        UCraftingComponent* CraftComp = PlayerChar->FindComponentByClass<UCraftingComponent>();
        if (CraftComp)
        {
            SaveInstance->SavedRecipes = CraftComp->GetRecipes();
        }

        UEquipmentComponent* EquipComp = PlayerChar->FindComponentByClass<UEquipmentComponent>();
        if (EquipComp)
        {
            SaveInstance->SavedEquippedItems = EquipComp->GetEquippedDataForSave();
        }

        if (UQuestComponent* QuestComp = PlayerChar->FindComponentByClass<UQuestComponent>())
        {
            QuestComp->GetQuestDataForSave(SaveInstance->SavedActiveQuests, SaveInstance->SavedCompletedQuests);
        }
    }

    // 2. 시간 데이터 저장 (월드에 배치된 TimeManager를 찾는 방식 예시)
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("TimeManager"), FoundActors);

    if (FoundActors.Num() > 0)
    {
        AActor* TimeManagerActor = FoundActors[0];

        // 2. 블루프린트의 CurrentTime 변수 값 읽기
        FProperty* Property = TimeManagerActor->GetClass()->FindPropertyByName(TEXT("CurrentTime"));
        if (Property)
        {
            // 1. Double(64비트)인 경우 (최신 언리얼 표준)
            if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Property))
            {
                SaveInstance->SavedWorldTime = (float)DoubleProp->GetPropertyValue_InContainer(TimeManagerActor);
            }
            // 2. Float(32비트)인 경우
            else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
            {
                SaveInstance->SavedWorldTime = FloatProp->GetPropertyValue_InContainer(TimeManagerActor);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("CurrentTime property found, but it's neither Double nor Float!"));
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("saveTimeIs failed1"));
    }

    // 3. 실제 파일 저장
    UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, 0);
}

void UMyGameInstance::LoadGame()
{
    // 1. 파일 존재 확인 및 로드
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return;

    UMySaveGame* LoadInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    if (!LoadInstance) return;

    // 2. 월드 내 플레이어 캐릭터 찾아서 데이터 복구
    ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerChar)
    {
        PlayerChar->SetActorLocation(LoadInstance->PlayerLocation);
        PlayerChar->SetActorRotation(LoadInstance->PlayerRotation);

        // 1. 스탯 복구
        if (UPlayerSurvivalStatComponent* StatComp = PlayerChar->FindComponentByClass<UPlayerSurvivalStatComponent>())
        {
            StatComp->LoadStats(LoadInstance->SavedStats);
        }

        // 2. 버프/디버프 복구
        if (UPlayerEffectComponent* EffectComp = PlayerChar->FindComponentByClass<UPlayerEffectComponent>())
        {
            // 중요: 기존에 걸려있던 효과들을 먼저 싹 지우고 로드하는 것이 안전할 수 있습니다.
            EffectComp->LoadEffects(LoadInstance->SavedActiveEffects);
        }
        

        // 인벤토리 복구
        UInventoryComponent* InvComp = PlayerChar->FindComponentByClass<UInventoryComponent>();
        if (InvComp)
        {
            InvComp->SetInventoryData(LoadInstance->SavedInventoryItems);
            InvComp->OnInventoryChanged.Broadcast(); // UI 갱신
        }

        // 크래프팅 복구
        UCraftingComponent* CraftComp = PlayerChar->FindComponentByClass<UCraftingComponent>();
        if (CraftComp)
        {
            CraftComp->SetRecipeData(LoadInstance->SavedRecipes);
            CraftComp->FOnCraftInventoryChange.Broadcast(); // UI 갱신
        }

        // 1. 장착 아이템 복구
        UEquipmentComponent* EquipComp = PlayerChar->FindComponentByClass<UEquipmentComponent>();
        if (EquipComp)
        {
            EquipComp->LoadEquippedItems(LoadInstance->SavedEquippedItems);
        }

        if (UQuestComponent* QuestComp = PlayerChar->FindComponentByClass<UQuestComponent>())
        {
            QuestComp->LoadQuestData(LoadInstance->SavedActiveQuests, LoadInstance->SavedCompletedQuests);
        }

        // 2. 시간 데이터 복구 및 라이팅 업데이트
        // 1. 월드에서 타임 매니저 찾기
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("TimeManager"), FoundActors);

        if (FoundActors.Num() > 0)
        {
            AActor* TimeManagerActor = FoundActors[0];

            // 2. 블루프린트의 CurrentTime 변수에 저장된 값 쓰기
            FProperty* Property = TimeManagerActor->GetClass()->FindPropertyByName(TEXT("CurrentTime"));
            if (Property)
            {
                if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Property))
                {
                    DoubleProp->SetPropertyValue_InContainer(TimeManagerActor, (double)LoadInstance->SavedWorldTime);
                }
                else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Property))
                {
                    FloatProp->SetPropertyValue_InContainer(TimeManagerActor, LoadInstance->SavedWorldTime);
                }
            }
        }
    
    }
}
