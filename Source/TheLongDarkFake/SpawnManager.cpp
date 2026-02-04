// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "NavigationSystem.h"
#include "Components/SphereComponent.h"
#include "TheLongDarkPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
ASpawnManager::ASpawnManager()
{
    PrimaryActorTick.bCanEverTick = false;

    ProximityTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ProximityTrigger"));
    ProximityTrigger->SetSphereRadius(10000.0f); // 100m
    ProximityTrigger->SetupAttachment(RootComponent);

    // 콜리전 이벤트 아예 끄기 (리소스 절약)
    ProximityTrigger->SetGenerateOverlapEvents(false);
    ProximityTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    

}



// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
    CheckWorldReady();
	
	
    
}



// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::ReturnAnimalToPool(FName AnimalID, AActor* Animal)
{
    if (!PoolMap.Contains(AnimalID)) return;

    FAnimalPoolGroup& PoolGroup = PoolMap[AnimalID];
    if (PoolGroup.ActivePool.Contains(Animal))
    {
        PoolGroup.ActivePool.Remove(Animal);
        PoolGroup.InactivePool.Add(Animal);

        // 비활성화
        Animal->SetActorHiddenInGame(true);
        Animal->SetActorEnableCollision(false);
        Animal->SetActorTickEnabled(false);
    }
}

void ASpawnManager::RespawnAnimalsAtMorning()
{
    for (auto& ConfigPair : AnimalConfigs)
    {
        FName ID = ConfigPair.Key;
        FAnimalPoolGroup& PoolGroup = PoolMap[ID];

        // 죽어있는(Inactive) 수만큼 다시 스폰
        int32 RespawnCount = PoolGroup.InactivePool.Num();
        for (int32 i = 0; i < RespawnCount; ++i)
        {
            ActivateFromPool(ID);
        }
    }
    GetWorldTimerManager().SetTimer(CheckDistanceTimer, this, &ASpawnManager::CheckPlayerDistance, 1.0f, true);
}

// 3. 거리 계산 로직 (1초에 한 번 실행)
void ASpawnManager::CheckPlayerDistance()
{
    // 깨울 동물이 없으면 타이머 종료 (최적화)
    if (SleepingAnimals.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
        return;
    }

    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (!PlayerPawn) return;

    // 제곱근 연산을 피한 거리 계산 (성능 최적화)
    float DistanceSq = FVector::DistSquared(GetActorLocation(), PlayerPawn->GetActorLocation());
    float TriggerRadiusSq = FMath::Square(ProximityTrigger->GetUnscaledSphereRadius());

    // 플레이어가 범위 안으로 들어왔는가?
    if (DistanceSq <= TriggerRadiusSq)
    {
        UE_LOG(LogTemp, Warning, TEXT("거리 체크 성공: 플레이어 접근 감지. 깨우기 시도."));
        WakeUpAnimals();
    }
}

// 4. 깨우기 및 바닥 확인 로직
void ASpawnManager::WakeUpAnimals()
{
    for (int32 i = SleepingAnimals.Num() - 1; i >= 0; --i)
    {
        AActor* Animal = SleepingAnimals[i];
        if (!Animal) continue;

        // 바닥 확인 (LineTrace)
        FHitResult Hit;
        FVector Start = Animal->GetActorLocation() + FVector(0, 0, 500);
        FVector End = Animal->GetActorLocation() - FVector(0, 0, 1000);

        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic))
        {
            // 바닥이 로드되었을 때만 활성화
            Animal->SetActorLocation(Hit.ImpactPoint + FVector(0, 0, 20));
            Animal->SetActorHiddenInGame(false);
            Animal->SetActorEnableCollision(true);
            Animal->SetActorTickEnabled(true);

            if (auto* MoveComp = Animal->FindComponentByClass<UCharacterMovementComponent>())
            {
                MoveComp->SetMovementMode(MOVE_Walking);
                MoveComp->GravityScale = 1.0f;
            }

            SleepingAnimals.RemoveAt(i);
        }
    }

    // 모두 깨웠다면 타이머 종료
    if (SleepingAnimals.Num() == 0)
    {
        GetWorldTimerManager().ClearTimer(CheckDistanceTimer);
    }
}

void ASpawnManager::CheckWorldReady()
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    bool bNavReady = NavSys && NavSys->GetDefaultNavDataInstance();

    // 내비메시가 아직 없다면 = 지형 로딩이 안 끝났다면
    if (!bNavReady)
    {
        UE_LOG(LogTemp, Warning, TEXT("월드 준비 중... 0.5초 뒤 재시도"));

        FTimerHandle RetryTimer;
        // 0.5초 뒤에 다시 이 함수를 호출 (준비될 때까지 무한 반복)
        GetWorldTimerManager().SetTimer(RetryTimer, this, &ASpawnManager::CheckWorldReady, 0.5f, false);
        return;
    }

    // [드디어 준비 완료!]
    UE_LOG(LogTemp, Warning, TEXT("월드 준비 완료! 동물을 소환합니다."));
    InitializePools();
    RespawnAnimalsAtMorning();
}

void ASpawnManager::InitializePools()
{
    // 무조건 스폰되도록 파라미터 설정
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (auto& ConfigPair : AnimalConfigs)
    {
        FName ID = ConfigPair.Key;
        FAnimalSpawnConfig& Config = ConfigPair.Value;
        FAnimalPoolGroup& PoolGroup = PoolMap.FindOrAdd(ID);

        for (int i = 0; i < Config.MaxCount; ++i)
        {
            AActor* NewAnimal = GetWorld()->SpawnActor<AActor>(Config.AnimalClass, FVector(0, 0, -5000), FRotator::ZeroRotator, SpawnParams);
            if (NewAnimal)
            {
                // 초기 상태: 비활성화
                NewAnimal->SetActorHiddenInGame(true);
                NewAnimal->SetActorEnableCollision(false);
                NewAnimal->SetActorTickEnabled(false);

                if (UCharacterMovementComponent* MoveComp = NewAnimal->FindComponentByClass<UCharacterMovementComponent>())
                {
                    MoveComp->StopMovementImmediately();
                    MoveComp->DisableMovement(); // 이동 모드를 None으로
                    MoveComp->GravityScale = 0.0f; // 중력을 아예 없앰
                }

                PoolGroup.InactivePool.Add(NewAnimal);
                
            }
        }
    }
}

void ASpawnManager::ActivateFromPool(FName AnimalID)
{
    if (!PoolMap.Contains(AnimalID)) return;
    FAnimalPoolGroup& PoolGroup = PoolMap[AnimalID];
    if (PoolGroup.InactivePool.Num() > 0)
    {
        AActor* Animal = PoolGroup.InactivePool.Pop();
        PoolGroup.ActivePool.Add(Animal);


        FVector TargetLoc = GetRandomNavLocation();

        // 디버그 로그: 위치를 찾았는지 못 찾았는지 출력
        if (TargetLoc.IsNearlyZero() || TargetLoc.Equals(GetActorLocation()))
        {
            UE_LOG(LogTemp, Error, TEXT("!!! %s 스폰 실패: 내비게이션 위치를 못 찾음 !!!"), *AnimalID.ToString());
        }
        TargetLoc.Z += 100.0f;

        
        // 2. 잠재우기 (핵심!)
        Animal->SetActorHiddenInGame(true);      // 보이지 않게
        Animal->SetActorEnableCollision(false);  // 바닥을 뚫고 떨어지지 않도록 물리 끔
        Animal->SetActorTickEnabled(false);      // AI나 로직 연산 중지

        if (UCharacterMovementComponent* MoveComp = Animal->FindComponentByClass<UCharacterMovementComponent>())
        {
            MoveComp->StopMovementImmediately(); // 기존 속도 제거
            MoveComp->DisableMovement();         // 이동 모드를 MOVE_None으로 변경
            MoveComp->GravityScale = 0.0f;       // 중력 제거
        }

        SleepingAnimals.Add(Animal);
        // 위치 설정 및 활성화
        Animal->SetActorLocation(TargetLoc, false, nullptr, ETeleportType::TeleportPhysics);
        // 실제 스폰된 위치 로그
        UE_LOG(LogTemp, Warning, TEXT("%s 활성화 완료 - 위치: %s"), *AnimalID.ToString(), *TargetLoc.ToString());
    }
}



FVector ASpawnManager::GetRandomNavLocation()
{
    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

    // 내비게이션 데이터가 로드될 때까지 기다려야 할 수도 있음
    if (!NavSys || !NavSys->GetDefaultNavDataInstance())
    {
        UE_LOG(LogTemp, Error, TEXT("NavData Instance 가 없습니다! 월드 파티션 로딩 대기 중..."));
        return FVector::ZeroVector;
    }

    if (NavSys)
    {
        FNavLocation RandomLoc;
        // 매니저 위치 기준 5000 유닛 반경 내 랜덤 위치 탐색
        if (NavSys->GetRandomReachablePointInRadius(GetActorLocation(), 5000.f, RandomLoc))
        {
            return RandomLoc.Location;
        }
    }
    
    return FVector::ZeroVector;
}

