// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CreatureData.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"     // UAnimMontage 클래스 사용을 위해 필요
#include "TimerManager.h"              // FTimerHandle 및 타이머 기능을 위해 필요
#include "AnimalBase.generated.h"


UCLASS()
class THELONGDARKFAKE_API AAnimalBase : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	class UDataTable* Creaturedatatable;
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	EBehaviorType m_Type{};
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FName m_Name{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float m_MaxHelth{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float m_CurHelth{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float m_DetectionRange{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TArray<FName> m_DropItem{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	int m_DropItemNum;
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float m_CombatDamage{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool m_IsDeath{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool m_bIsAttackMode{};
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float m_CurSpeed{};



public:
	// Sets default values for this character's properties
	AAnimalBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	// 공격 중인지 확인하는 변수 (애니메이션 몽타주와 연동용)
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float SideDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ItemClass; // 실제 월드에 스폰될 아이템 블루프린트 클래스

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	// AI 컨트롤러에서 호출할 Getter
	class UBehaviorTree* GetBTAsset() const { return BehaviorTreeAsset; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// 에디터에서 공격 몽타주를 할당할 수 있게 합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;
	// 타이머 핸들 선언
	FTimerHandle DeathTimerHandle;
	void DestroyAnimal();

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* ItemDropTable;

	// 아이템을 스폰하는 함수
	void DropItem();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetMovementSpeed(float NewSpeed);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void Attack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackHitCheck();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRange = 400.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 100.0f;

	// 공격 종료 시 호출할 함수 (애니메이션 노티파이 등에서 사용)
	UFUNCTION(BlueprintCallable, Category = "AI")
	void StopAttacking();

	// 타이머 핸들 (공격 종료 시점 관리)
	FTimerHandle AttackTimerHandle;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EBehaviorType GetAnimalType();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void TakepriDamage(float Damage);
	UFUNCTION(BlueprintCallable)
	void SetIsDeath(bool death);
	UFUNCTION(BlueprintCallable)
	bool GetIsDeath();
	UFUNCTION(BlueprintCallable)
	float GetCurHealth();
	
	UFUNCTION(BlueprintCallable)
	void SetIsAttack(bool AttackMode);
	UFUNCTION(BlueprintCallable)
	bool GetIsAttack();

	void InitAnimal();
};
