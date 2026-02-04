// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    //Detect(); // 매 틱마다 감지
	// ...
}

void UInteractionComponent::Detect()
{
    // 1. 트레이스 시작/종료 지점 계산 (액터 위치와 전방 벡터 사용)
    FVector StartLocation = GetOwner()->GetActorLocation();
    FVector ForwardVector = GetOwner()->GetActorForwardVector();
    FVector EndLocation = StartLocation + (ForwardVector * TraceDistance);

    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());

    // 2. Sphere Trace 실행 
    bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),StartLocation, EndLocation, TraceRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

    AActor* HitActor = bHit ? HitResult.GetActor() : nullptr;
    AActor* NewDetectedActor = nullptr;

    if (HitActor)
    {
        // 3. 히트된 액터가 IInteractableInterface를 구현했는지 확인
        if (HitActor->Implements<UInteractableInterface>())
        {
            NewDetectedActor = HitActor;
        }
    }

    // 4. 상태 업데이트
    if (DetectedActor.Get() != NewDetectedActor) // TWeakObjectPtr 비교는 .Get()을 사용
    {
        DetectedActor = NewDetectedActor;
        // HUD 업데이트 로직 추가...
    }
}

void UInteractionComponent::RequestInteraction()
{
    Detect();

    AActor* TargetActor = GetDetectedInteractableActor(); 
    
    if (TargetActor)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("sucsess targeting"), false);
        // TargetActor는 이미 IInteractableInterface를 구현한 것이 검증된 상태이므로,
        // 인터페이스 함수를 안전하게 호출합니다.
        
        // Execute_Interact는 인터페이스가 C++로 선언되었을 때 사용하는 함수 호출 방식입니다.
        // 첫 번째 인수는 대상 액터, 두 번째 인수는 상호작용 주체(플레이어 캐릭터)입니다.
        IInteractableInterface::Execute_Interact(TargetActor, GetOwner());

        // 상호작용 실행 후, DetectedActor를 초기화하여 UI가 사라지게 합니다.
        // 획득 성공 시 액터 파괴는 AItemBase::Interact_Implementation에서 처리합니다.
        DetectedActor = nullptr;
    }
    else
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Blue, TEXT("false targeting"), false);
    }
    
}

void UInteractionComponent::setIsinteract(bool curinteract)
{
    m_bIsInteract = curinteract;
}

bool UInteractionComponent::GetIsinteract()
{
    return m_bIsInteract;
}


