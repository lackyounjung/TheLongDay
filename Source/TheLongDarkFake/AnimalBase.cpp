// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalBase.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TheLongDarkPlayer.h"
#include "BrainComponent.h" // 이 헤더를 반드시 추가하세요!
#include "Item_Data.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AnimalAIController.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "QuestComponent.h"
#include "PlayerEffectComponent.h"
#include "SpawnManager.h"

// Sets default values
AAnimalBase::AAnimalBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // **캡슐 컴포넌트 크기 조정**

    // 1. GetCapsuleComponent() 함수를 사용하여 캡슐 컴포넌트에 접근합니다.
    UCapsuleComponent* Capsule = GetCapsuleComponent();

    // 2. 늑대나 사슴과 같은 4족 보행 동물에 맞는 크기로 설정합니다.
    // (예: 높이 45, 반지름 40으로 설정)
    Capsule->SetCapsuleHalfHeight(0.0f); // 캡슐의 높이 절반 (총 높이 90)
    Capsule->SetCapsuleRadius(40.0f);     // 캡슐의 반지름 (너비)

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    // ===============================================
    // **AI 컨트롤러 설정**
    // ===============================================

    // 2. 이 캐릭터를 소유할 AI Controller 클래스를 설정합니다.
    // 여기에 사용자가 만든 AIController 클래스를 지정해야 합니다.
    AIControllerClass = AAnimalAIController::StaticClass();

    // 3. 캐릭터가 월드에 배치되거나 스폰될 때 AI 컨트롤러가 자동으로 소유하도록 설정합니다.
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // 4. 캐릭터가 플레이어에 의해 컨트롤되지 않도록 설정 (선택 사항)
    // AI 전용 캐릭터라면 이 설정을 확인하는 것이 좋습니다.
    AutoPossessPlayer = EAutoReceiveInput::Disabled;

    

    
}

// Called when the game starts or when spawned
void AAnimalBase::BeginPlay()
{
	Super::BeginPlay();
    if (Creaturedatatable)
    {
        if (const FCreatureData* MasterRow = Creaturedatatable->FindRow<FCreatureData>(m_Name, TEXT("Find Master Item Data")))
        {
            m_MaxHelth = MasterRow->MaxHealth;
            m_CurHelth = m_MaxHelth;
            m_DetectionRange = MasterRow->DetectionRange;
            m_CombatDamage = MasterRow->CombatDamage;
            m_DropItem = MasterRow->DropItemID;
            m_DropItemNum = MasterRow->DropItemID.Num();
            m_Type = MasterRow->BehaviorType;
            m_CurSpeed = 100;
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Creaturedatatable is Missing!"));
    }
	
}



void AAnimalBase::DropItem()
{
    // 1. 데이터테이블과 드롭할 아이템 이름이 유효한지 확인
    for (int i = 0; i < m_DropItemNum; i++)
    {
        if (!ItemDropTable || m_DropItem[i].IsNone())
        {
            UE_LOG(LogTemp, Warning, TEXT("DropTable or DropItemID is Missing!"));
            return;
        }

        // 2. 데이터테이블에서 m_DropItem(이름)에 해당하는 행을 찾음
        static const FString ContextString(TEXT("Item Drop Context"));
        FItem_Data* DropRow = ItemDropTable->FindRow<FItem_Data>(m_DropItem[i], ContextString);

        if (DropRow && DropRow->ItemActorClass)
        {
            // 3. 스폰 위치 설정 (동물의 위치에서 약간 위쪽)
            FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 20.0f);
            FRotator SpawnRotation = FRotator::ZeroRotator;

            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 4. 월드에 아이템 액터 스폰
            AActor* DroppedItem = GetWorld()->SpawnActor<AActor>(DropRow->ItemActorClass, SpawnLocation, SpawnRotation, SpawnParams);

            if (DroppedItem)
            {
                UE_LOG(LogTemp, Warning, TEXT("Successfully Dropped: %s"), *m_DropItem[i].ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find ItemID: %s in DropTable"), *m_DropItem[i].ToString());
        }

    }
    

    
}

// Called every frame
void AAnimalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GetCharacterMovement())
    {
        // 데이터테이블의 달리기 속도 값을 적용
        //GetCharacterMovement()->MaxWalkSpeed = m_CurSpeed; // 실제로는 변수값을 넣으세요.
        FVector Velocity = GetVelocity();
        FRotator Rotation = GetActorRotation();

        SideDirection = GetDefault<UAnimInstance>()->CalculateDirection(Velocity, Rotation);
        
    }

}

void AAnimalBase::SetMovementSpeed(float NewSpeed)
{
    if (GetCharacterMovement())
    {
        m_CurSpeed = NewSpeed;
        GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
    }
}

void AAnimalBase::Attack()
{
    // 로그를 찍어서 이 함수가 실제로 호출되는지, 몽타주가 유효한지 확인해보세요.
        if (AttackMontage)
        {
            UE_LOG(LogTemp, Warning, TEXT("Montage Playing!")); // 출력 창에서 확인
            PlayAnimMontage(AttackMontage, 1.0f);
            //GetCharacterMovement()->StopMovementImmediately();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AttackMontage is NULL!"));
        }
}

void AAnimalBase::AttackHitCheck()
{
    FVector Start = GetActorLocation() + GetActorForwardVector() * 50.0f +FVector{0,0,70.f};
    FVector End = Start + GetActorForwardVector() * AttackRange;

    // 2. 무시할 액터 설정 (자기 자신은 때리면 안 됨)
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    FHitResult HitResult;

    // 3. 구체 트레이스 실행 (시각적으로 확인 가능하게 보라색 구체를 그림)
    bool bHit = UKismetSystemLibrary::SphereTraceSingle(
        GetWorld(),
        Start,
        End,
        AttackRadius,
        UEngineTypes::ConvertToTraceType(ECC_Pawn), // Pawn 채널 감지
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None, // 디버그용 구체를 3초간 표시
        HitResult,
        true,
        FLinearColor::Red,
        FLinearColor::Green,
        3.0f
    );

    // 4. 충돌 시 대미지 전달
    if (bHit && HitResult.GetActor())
    {
        AActor* HitActor = HitResult.GetActor();
        UE_LOG(LogTemp, Warning, TEXT("Hit Target: %s"), *HitActor->GetName());

        // 1. 맞은 액터로부터 해당 클래스의 컴포넌트를 찾아옵니다.
        UPlayerSurvivalStatComponent* StatComp = HitActor->FindComponentByClass<UPlayerSurvivalStatComponent>();
        UPlayerEffectComponent* EffectComp = HitActor->FindComponentByClass<UPlayerEffectComponent>();

        if (StatComp)
        {
            // 2. 컴포넌트를 찾았다면 원하는 함수 호출
            // 예: StatComp->DecreaseHealth(AttackDamage);
            StatComp->ChangeStatValue("Health",-1* m_CombatDamage);
            UE_LOG(LogTemp, Warning, TEXT("Successfully found StatComponent on %s"), *HitActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find UPlayerSurvivalStatComponent on %s"), *HitActor->GetName());
        }

        if (EffectComp)
        {
            EffectComp->ApplyEffect("bleeding");
            UE_LOG(LogTemp, Warning, TEXT("Successfully found EffectComponent on %s"), *HitActor->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Could not find UPlayerEffectComponent on %s"), *HitActor->GetName());
        }
    }
}

void AAnimalBase::StopAttacking()
{
    bIsAttacking = false;
}

// Called to bind functionality to input
void AAnimalBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

EBehaviorType AAnimalBase::GetAnimalType()
{
    return m_Type;
}

void AAnimalBase::TakepriDamage(float Damage)
{
    m_CurHelth -= Damage;
    // 데미지가 들어올 때마다 로그 찍기
    UE_LOG(LogTemp, Warning, TEXT("Remaining HP: %f"), m_CurHelth);
    if (m_CurHelth <= 0)
    {
        SetIsDeath(true);
    }
}

void AAnimalBase::SetIsDeath(bool death)
{
    m_IsDeath = death;
    if (death)
    {
        

        // 1. 로그 확인
        UE_LOG(LogTemp, Warning, TEXT("!!!Animal is Dead !!!"));
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        UQuestComponent* questcomp = PlayerPawn->FindComponentByClass<UQuestComponent>();
        questcomp->NotifyAction(EQuestObjectiveType::Hunt, m_Name);

        // 2. AI 컨트롤러 제어
        AAIController* AIController = Cast<AAIController>(GetController());
        if (AIController)
        {
            // 블랙보드 값 변경 (BT 데코레이터가 인지하도록)
            if (AIController->GetBlackboardComponent())
            {
                AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDeath"), true);
            }

            // AI 로직(뇌) 즉시 정지
            if (AIController->GetBrainComponent())
            {
                AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));
            }

            // 현재 진행 중인 모든 길찾기 이동 명령 취소
            AIController->StopMovement();
        }

        // 3. 물리적 이동 차단
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->StopMovementImmediately(); // 즉시 정지
            GetCharacterMovement()->DisableMovement();         // 이후 이동 불가
        }

        // 4. 충돌 해제 (플레이어가 죽은 시체를 밀고 다닐 수 없게)
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    
    // 아이템 드롭 실행
    DropItem();

    // 5초 뒤에 액터를 자동으로 Destroy 합니다.
    GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AAnimalBase::DestroyAnimal, 5.0f, false);
    
}

bool AAnimalBase::GetIsDeath()
{
    return m_IsDeath;
}

float AAnimalBase::GetCurHealth()
{
    return m_CurHelth;
}

void AAnimalBase::SetIsAttack(bool AttackMode)
{
    m_bIsAttackMode = AttackMode;
}

bool AAnimalBase::GetIsAttack()
{
    return m_bIsAttackMode;
}

void AAnimalBase::InitAnimal()
{
    // 1. 상태 변수 초기화
    m_IsDeath = false;
    m_CurHelth = m_MaxHelth;
    bIsAttacking = false;
    m_bIsAttackMode = false;

    // 2. 물리 및 충돌 복구
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block); // 기본 설정에 맞춰 수정
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->SetDefaultMovementMode();
    }

    // 3. AI 다시 켜기
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        if (AIController->GetBlackboardComponent())
        {
            AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDeath"), false);
        }
        if (AIController->GetBrainComponent())
        {
            AIController->GetBrainComponent()->RestartLogic();
        }
    }
}

void AAnimalBase::DestroyAnimal()
{
    // 나에게 붙어있는 모든 액터를 가져옵니다. (화살 등)
    TArray<AActor*> AttachedActors;
    GetAttachedActors(AttachedActors);

    for (AActor* Victim : AttachedActors)
    {
        if (Victim)
        {
            Victim->Destroy(); // 화살 먼저 파괴
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Wolf and its Arrows are destroyed."));
    
    if (m_Name != "Bear_Black")
    {
        // 2. 스폰 매니저를 찾아서 반납
        AActor* ManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnManager::StaticClass());
        ASpawnManager* SpawnManager = Cast<ASpawnManager>(ManagerActor);

        if (SpawnManager)
        {
            // 매니저의 반납 함수 호출 (ID는 데이터테이블의 m_Name 등을 활용)
            SpawnManager->ReturnAnimalToPool(m_Name, this);
            UE_LOG(LogTemp, Warning, TEXT("%s returned to pool."), *m_Name.ToString());
        }
        else
        {
            // 매니저를 못 찾으면 안전을 위해 Destroy
            Destroy();
        }
    }
    /*else
    {
        Destroy();
    }*/

    
}

