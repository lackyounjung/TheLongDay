// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

// 동물별 설정 (에디터 노출용)
USTRUCT(BlueprintType)
struct FAnimalSpawnConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<AActor> AnimalClass;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 MaxCount;
};

// 풀링 저장소
USTRUCT(BlueprintType)
struct FAnimalPoolGroup
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> InactivePool; // 대기 중인 동물

	UPROPERTY()
	TArray<AActor*> ActivePool;   // 필드에 나온 동물
};


UCLASS()
class THELONGDARKFAKE_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 플레이어 감지용 트리거
	UPROPERTY(VisibleAnywhere, Category = "Optimization")
	class USphereComponent* ProximityTrigger;

	// 현재 잠들어 있는 동물들 리스트
	UPROPERTY()
	TArray<AActor*> SleepingAnimals;

	// [추가] 거리 체크 타이머를 제어하기 위한 핸들
	FTimerHandle CheckDistanceTimer;

	// [추가] 거리 체크 함수
	void CheckPlayerDistance();

	// [추가] 동물 활성화 함수
	void WakeUpAnimals();

	void CheckWorldReady();

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// --- 풀링 시스템 핵심 함수 ---

	// 1. 에디터 설정용 맵 (Key: 동물 이름, Value: 설정)
	UPROPERTY(EditAnywhere, Category = "Animal Settings")
	TMap<FName, FAnimalSpawnConfig> AnimalConfigs;

	// 2. 동물이 죽었을 때 외부에서 호출 (반납)
	UFUNCTION(BlueprintCallable, Category = "SpawnSystem")
	void ReturnAnimalToPool(FName AnimalID, AActor* Animal);

	// 3. 아침 리스폰 이벤트 (시간 시스템 연동용)
	UFUNCTION(BlueprintCallable, Category = "SpawnSystem")
	void RespawnAnimalsAtMorning();
private:
	// 실제 풀 데이터를 들고 있는 변수
	UPROPERTY()
	TMap<FName, FAnimalPoolGroup> PoolMap;

	void InitializePools();
	void ActivateFromPool(FName AnimalID);
	

	// 안전한 위치 계산 (NavMesh 활용)
	FVector GetRandomNavLocation();

	

	

};
