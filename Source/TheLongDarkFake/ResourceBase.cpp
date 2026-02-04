// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceBase.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "ResourceData.h"
#include "TheLongDarkPlayer.h"
#include "LoadingWidget.h"
#include "EquipmentComponent.h"
#include "InteractionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"





// Sets default values
AResourceBase::AResourceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// UStaticMeshComponent를 생성합니다.
	ResourceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ressourceMesh"));

    RootComponent = ResourceMesh;
	

}

// Called when the game starts or when spawned
void AResourceBase::BeginPlay()
{
	Super::BeginPlay();
    
    if (ResourceMesh && ResourceMesh->GetMaterial(0))
    {
        // 런타임에 머티리얼을 동적으로 제어하기 위해 인스턴스 생성
        DynamicMaterialInstance = ResourceMesh->CreateDynamicMaterialInstance(0);

        if (DynamicMaterialInstance)
        {
            // 초기값 설정 (파괴되지 않은 상태)
            DynamicMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), -1.0f);
        }
    }
   
    if (ResourceDataTabl)
    {
        if (const FResourceData* MasterRow = ResourceDataTabl->FindRow<FResourceData>(m_ResourceID, TEXT("Find ResourceData")))
        {
            if (MasterRow->DropItemID.Num() > 0)
            {
                m_ItemsName = MasterRow->DropItemID;
                // 2. 인덱스 접근 전 한 번 더 안전하게 체크
                if (m_ItemsName.IsValidIndex(0))
                {
                    m_DropItemName = m_ItemsName[0];
                }
            }
            
        }
        else
        {
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("false finde row"), false);
        }
    }
    if (dropitemDataTabl)
    {
        if (const FItem_Data* MasterRow = dropitemDataTabl->FindRow<FItem_Data>(m_DropItemName, TEXT("Find dropitemData")))
        {
            DropItemActorClass = MasterRow->ItemActorClass;
            
        }
    }
}

// Called every frame
void AResourceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    

}

FName AResourceBase::GetResourceName()
{
	return m_ResourceID;
}

bool AResourceBase::Interact_Implementation(AActor* player)
{


    ATheLongDarkPlayer* pPlayer = Cast<ATheLongDarkPlayer>(player);
    if (!pPlayer) return false;

    UEquipmentComponent* EquipComp = pPlayer->FindComponentByClass<UEquipmentComponent>();
    UInteractionComponent* InteractComp = pPlayer->FindComponentByClass<UInteractionComponent>();

    if (EquipComp->GetCurWeapon() != EWeaponType::ET_Axe)
        return false;
    InteractComp->setIsinteract(true);
    

    InstigatorActor = player;

    CurrentGatheringTime = 0.0f;

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

    pPlayer->SetInteraction(true);
    pPlayer->SetCanMove(false);
    PC->SetIgnoreLookInput(true);

    //if (LoadingWidgetClass)
    //{
    //    
    //    if (PC)
    //    {
    //        LoadingWidgetInstance = CreateWidget<ULoadingWidget>(PC, LoadingWidgetClass);
    //        if (LoadingWidgetInstance)
    //        {
    //            LoadingWidgetInstance->AddToViewport();
    //        }
    //        else
    //        {
    //            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("failed!"), false);
    //        }
    //    }
    //}

    // 델리게이트 생성: 타이머가 호출할 함수를 바인딩합니다.
    FTimerDelegate TimerDelegate;
    // this 포인터와 클래스 멤버 함수를 안전하게 바인딩합니다.
    TimerDelegate.BindUObject(this, &AResourceBase::UpdateDissolveEffect);

    // 타이머 설정
    // 1. TimerHandle: 타이머 관리 핸들
    // 2. TimerDelegate: 호출할 함수
    // 3. UpdateInterval: 반복 간격 (0.1초)
    // 4. bLoop: 반복 실행 여부 (!!! 핵심: true로 설정)
    GetWorldTimerManager().SetTimer(
        DepletionTimerHandle,
        TimerDelegate,
        TimerUpdateInterval,
        true
    );

    // 상호작용이 완료되지 않았으므로 false 반환
    return false;


}

bool AResourceBase::DropItem_Implementation(AActor* player)
{
    const FVector SpawnLocation = GetActorLocation();
    const FRotator SpawnRotation = GetActorRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = this;
    AActor* DroppedItem = GetWorld()->SpawnActor<AActor>(DropItemActorClass,SpawnLocation, SpawnRotation,SpawnParams);
    return false;
}



void AResourceBase::UpdateDissolveEffect()
{
    CurrentGatheringTime += TimerUpdateInterval;
    float Progress = FMath::Clamp(CurrentGatheringTime / TotalGatheringTime, 0.0f,1.0f);

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

    /*if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), Progress);
    }*/

    //if (LoadingWidgetInstance)
    //{
    //    
    //    if (LoadingWidgetInstance->CallBlueprintFunction(Progress))
    //    {
    //        //FString DebugMessage = FString::Printf(TEXT("progress: %f"), Progress);

    //        // 화면에 출력
    //        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, DebugMessage, false);
    //        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("progress!"), false);
    //    }
    //    
    //}

    if (Progress >= 1.0f)
    {
        // 타이머 중지 (반복을 멈춥니다)
        GetWorldTimerManager().ClearTimer(DepletionTimerHandle);

        // 액터 유효성 재확인 (안전 장치)
        if (!IsValid(this)) return;

        /*LoadingWidgetInstance->RemoveFromParent();
        LoadingWidgetInstance = nullptr;*/

        AActor* PlayerActor = InstigatorActor.Get();

        if (!PlayerActor || !IsValid(PlayerActor)) return;

        // Inventory에 아이템 추가 (여기에 성공/실패 로직 포함)
        UInventoryComponent* Inventory = PlayerActor->FindComponentByClass<UInventoryComponent>();

        if (Inventory && Inventory->AddItem(m_DropItemName))
        {
            // 성공 메시지
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("채집 완료 및 획득 성공!"), false);

            // 액터 파괴
            Destroy();
        }
        else
        {
            // 실패 메시지 (인벤토리 꽉 참 등)
            //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("채집 실패 또는 인벤토리 부족."), false);
        }

        // 플레이어 상태 복구
        ATheLongDarkPlayer* pPlayer = Cast<ATheLongDarkPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
        UInteractionComponent* InteractComp = pPlayer->FindComponentByClass<UInteractionComponent>();
        if (pPlayer)
        {
            pPlayer->SetInteraction(false);
            pPlayer->SetCanMove(true);
            PC->SetIgnoreLookInput(false);
            InteractComp->setIsinteract(false);

        }
    }
}

