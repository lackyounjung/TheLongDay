// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TheLongDarkPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" // 블랙보드 사용을 위해 추가
#include "BehaviorTree/BehaviorTreeComponent.h" // 비헤이비어 트리 컴포넌트 사용을 위해 추가
#include "AnimalBase.h" // m_Type에 접근하기 위해 추가
#include "CreatureData.h" // EBehaviorType 사용을 위해 추가

AAnimalAIController::AAnimalAIController()
{
    // 1. 컴포넌트 생성
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

    // 2. 시각 감지 설정 (Configuration) 생성
    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

    // 3. 감지 가능한 시야 범위 설정 (예시 값)
    SightConfig->SightRadius = 1500.0f;           // 일반 시야 거리
    SightConfig->LoseSightRadius = 2000.0f;       // 시야를 잃는 거리
    SightConfig->PeripheralVisionAngleDegrees = 120.0f; // 시야 각도 (90도)

    // 4. 감지 성공 시 이 컨트롤러에 보고되도록 설정
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    // 5. Perception 컴포넌트에 시각 설정 등록
    PerceptionComp->ConfigureSense(*SightConfig);
    //PerceptionComp->SetPrimarySense(*SightConfig->GetSenseImplementation()); // 주 감지 센서로 설정

    // 6. 감지 업데이트 이벤트에 함수 바인딩
    PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AAnimalAIController::OnTargetPerceptionUpdated);

    // 이외에도 Hearing(청각) 등을 추가할 수 있습니다.
    // **추가: 비헤이비어 트리 컴포넌트 생성**
    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAnimalAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AAnimalBase* AnimalPawn = Cast<AAnimalBase>(InPawn);

    if (AnimalPawn && AnimalPawn->GetBTAsset())
    {
        // 1. 임시 일반 포인터 변수를 하나 만듭니다.
        UBlackboardComponent* TempBlackboard = nullptr;

        // 2. 임시 변수를 인자로 전달합니다. (타입이 정확히 일치하게 됨)
        if (UseBlackboard(AnimalPawn->GetBTAsset()->BlackboardAsset, TempBlackboard))
        {
            // 3. 성공했다면 멤버 변수 Blackboard(TObjectPtr)에 대입해줍니다.
            Blackboard = TempBlackboard;

            if (Blackboard)
            {
                Blackboard->SetValueAsInt(TEXT("BehaviorType"), (int32)AnimalPawn->GetAnimalType());
            }

            RunBehaviorTree(AnimalPawn->GetBTAsset());
        }
    }
}

void AAnimalAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // **핵심: 타겟 감지 시 TargetActor 블랙보드 키 설정 로직**
    

    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
    if (!BlackboardComp || !Actor) return;
    ATheLongDarkPlayer* PlayerCharacter = Cast<ATheLongDarkPlayer>(Actor);

    if (Stimulus.WasSuccessfullySensed())
    {
        // 2. 플레이어일 때만 TargetActor로 설정
        if (PlayerCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player Sensed: %s"), *Actor->GetName());
            BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
        }
    }
    else
    {
        // 시야에서 사라진 것이 플레이어라면 TargetActor 해제
        if (PlayerCharacter)
        {
            BlackboardComp->ClearValue(TEXT("TargetActor"));
        }
    }

    //if (BlackboardComp)
    //{
    //    if (Stimulus.WasSuccessfullySensed())
    //    {
    //        // 로그를 찍어서 실제로 감지됐는지 출력창(Output Log)에서 확인하세요.
    //        UE_LOG(LogTemp, Warning, TEXT("Target Sensed: %s"), *Actor->GetName());
    //        // 감지에 성공하면 TargetActor 키에 감지된 액터를 설정
    //        BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
    //    }
    //    else
    //    {
    //        // 시야에서 놓쳤다면 TargetActor 키를 해제
    //        // (주의: 공격이 끝난 후 ClearValue는 다른 태스크에서 처리하는 것이 더 좋습니다.)
    //         BlackboardComp->ClearValue(TEXT("TargetActor")); 
    //    }
    //}
}
