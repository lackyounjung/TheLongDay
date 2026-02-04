


#include "PlayerSurvivalStatComponent.h"
#include "GameFramework/Character.h" // GetOwner()를 Cast<ACharacter>하기 위해
#include "PlayerEffectComponent.h"

void UPlayerSurvivalStatComponent::InitializeStats()
{
	// 1. 데이터 테이블 포인터 유효성 검사
	if (!PlayerStatsDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[SurvivalStatComponent] PlayerStatsDataTable not set! Check component details panel."));
		return;
	}

	// 2. 데이터 테이블의 모든 행을 읽어와 SurvivalStats 배열에 복사
	FString ContextString;
	TArray<FPlayerStat*> OutRows;
	PlayerStatsDataTable->GetAllRows<FPlayerStat>(ContextString, OutRows);

	for (FPlayerStat* Row : OutRows)
	{
		FCurrentStat NewStat;
		NewStat.MasterData = *Row; // 마스터 데이터 복사
		NewStat.CurrentValue = Row->InitialValue; // 시작 값으로 초기화
		SurvivalStats.Add(NewStat);

		UE_LOG(LogTemp, Log, TEXT("Stat Initialized: %s, Rate: %.3f"),
			*Row->StatID.ToString(), Row->DecreaseRatePerSec);
	}
}

void UPlayerSurvivalStatComponent::DecreaseStats()
{
	// 소유자 캐릭터와 이펙트 컴포넌트를 미리 캐싱합니다.
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	UPlayerEffectComponent* EffectComp = OwnerCharacter->FindComponentByClass<UPlayerEffectComponent>();
	// 컴포넌트가 없으면 디버프 적용 로직은 실행할 수 없습니다.
	if (!EffectComp) return;

	// 모든 스탯을 순회하며 감소 로직 적용
	for (FCurrentStat& Stat : SurvivalStats)
	{
		// 1. 초당 감소율 적용
		Stat.CurrentValue -= Stat.MasterData.DecreaseRatePerSec;

		// 2. 0 이하로 떨어지지 않도록 clamp (최소값 0)
		Stat.CurrentValue = FMath::Clamp(Stat.CurrentValue, 0.0f, Stat.MasterData.MaxValue);

		// 3. 위험 임계점 체크 로직 (2단계의 핵심: 디버프 적용)
		if (Stat.CurrentValue <= Stat.MasterData.CriticalThreshold)
		{
			// 위험 임계점 아래인데, 아직 디버프가 적용되지 않았다면 적용
			if (!Stat.bIsDebuffActive && !Stat.MasterData.DebuffEffectID.IsNone())
			{
				EffectComp->ApplyEffect(Stat.MasterData.DebuffEffectID);
				Stat.bIsDebuffActive = true; // 플래그 설정
			}
			
			
		}
		else if (Stat.CurrentValue > Stat.MasterData.CriticalThreshold)
		{
			// 임계점 위로 회복했고, 디버프가 적용 중이라면 제거
			if (Stat.bIsDebuffActive && !Stat.MasterData.DebuffEffectID.IsNone())
			{
				EffectComp->RemoveEffect(Stat.MasterData.DebuffEffectID);
				Stat.bIsDebuffActive = false; // 플래그 해제
			}
		}
	}
}

float UPlayerSurvivalStatComponent::GetStatCurrentValue(FName StatID) const
{
	for (const FCurrentStat& Stat : SurvivalStats)
	{
		if (Stat.MasterData.StatID == StatID)
		{
			return Stat.CurrentValue;
		}
	}
	return 0.0f;
	
}

float UPlayerSurvivalStatComponent::GetStatMaxValue(FName StatID) const
{
	for (const FCurrentStat& Stat : SurvivalStats)
	{
		if (Stat.MasterData.StatID == StatID)
		{
			return Stat.MasterData.MaxValue;
		}
	}
	return 1.0f;
}

void UPlayerSurvivalStatComponent::ChangeStatValue(FName StatID, float Amount)
{
	// 음식 섭취 (양수 Amount), 피해 (음수 Amount) 로직
	for (FCurrentStat& Stat : SurvivalStats)
	{
		if (Stat.MasterData.StatID == StatID)
		{
			Stat.CurrentValue += Amount;
			Stat.CurrentValue = FMath::Clamp(Stat.CurrentValue, 0.0f, Stat.MasterData.MaxValue);

			// TODO: UI가 업데이트되도록 이벤트 또는 바인딩을 리프레시하는 로직 추가
			return;
		}
	}
}

void UPlayerSurvivalStatComponent::ChangeDecreaseStatValue(FName StatID, float Amount)
{
	for (FCurrentStat& Stat : SurvivalStats)
	{
		if (Stat.MasterData.StatID == StatID)
		{
			Stat.MasterData.DecreaseRatePerSec += Amount;
			Stat.CurrentValue = FMath::Clamp(Stat.CurrentValue, 0.0f, Stat.MasterData.MaxValue);

			// TODO: UI가 업데이트되도록 이벤트 또는 바인딩을 리프레시하는 로직 추가
			return;
		}
	}
}

// Sets default values for this component's properties
UPlayerSurvivalStatComponent::UPlayerSurvivalStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPlayerSurvivalStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// 1. 데이터 테이블에서 스탯 정보를 읽어와 초기화
	InitializeStats();

	// 2. 스탯 감소 타이머 시작 (1초마다 DecreaseStats 함수 호출)
	if (GetWorld() && SurvivalStats.Num() > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(StatDecreaseTimerHandle,this,&UPlayerSurvivalStatComponent::DecreaseStats,1.0f, true ); //1초 반복
	}
	
}


// Called every frame
void UPlayerSurvivalStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

