// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AnimalAIController.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API AAnimalAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAnimalAIController();

private:
	// **추가: 비헤이비어 트리 컴포넌트**
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BehaviorTreeComp;
	// AI가 시야, 청각 등을 통해 대상을 감지하도록 하는 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* PerceptionComp;

protected:
	// **추가: OnPossess 오버라이드** (AI 컨트롤러가 Pawn을 소유할 때 호출)
	virtual void OnPossess(APawn* InPawn) override;
	// 감지 이벤트 발생 시 호출될 함수
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	FORCEINLINE class UAIPerceptionComponent* GetPerceptionComp() const { return PerceptionComp; }
	
};
