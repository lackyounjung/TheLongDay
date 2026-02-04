// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToTarget.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
    // 태스크의 이름 설정 (에디터에서 보임)
    NodeName = "Move To Location";

    // TargetLocationKey를 Vector 타입으로 초기화 (블랙보드 키 설정)
    
    BlackboardKey.AddVectorFilter(this, FName("BlackboardKey"));
    BlackboardKey.AddObjectFilter(this, FName("BlackboardKey"), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 1. AI 컨트롤러와 AI 소유 액터(동물) 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

    if (!AIController || !AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    // 2. 블랙보드에서 목표 위치 값(Vector) 가져오기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    FVector TargetLocation = BlackboardComp->GetValueAsVector(TargetLocationKey.SelectedKeyName);

    // 3. AI 이동 명령 실행
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalLocation(TargetLocation);
    MoveRequest.SetAcceptanceRadius(50.0f); // 목표 근처 50cm 안에 도착하면 성공으로 간주

    // 이동 명령을 비동기(Async)로 실행
    FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest);

    // 4. 이동 명령 결과에 따라 분기
    if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
    {
        // 이동이 시작되면, 완료될 때까지 이 태스크는 대기 상태에 있어야 합니다.
        // 이동이 완료되거나 실패하면 (Delegate를 통해) EndTask() 함수가 호출됩니다.
        return EBTNodeResult::InProgress;
    }

    return EBTNodeResult::Failed;
}
