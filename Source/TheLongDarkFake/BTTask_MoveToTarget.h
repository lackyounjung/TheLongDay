// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToTarget.generated.h"


/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UBTTask_MoveToTarget : public UBTTask_BlackboardBase
{	
    GENERATED_BODY()
public:
    UBTTask_MoveToTarget();

protected:
    // **가장 중요한 함수: 태스크가 실행될 때 호출됩니다.**
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 이동 목표 위치를 블랙보드에서 가져오기 위한 키입니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
    struct FBlackboardKeySelector TargetLocationKey;
};
