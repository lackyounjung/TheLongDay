// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildComponent.h"
#include "Kismet/GameplayStatics.h" // UGameplayStatics 사용을 위해 필요
#include "BuildItemBase.h"
#include "GameFramework/PlayerController.h"


void UBuildComponent::UpdateMouseSnapLocation()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult HitResult;
		// 마우스 커서 아래에 있는 물체와의 충돌 지점을 가져옴
		if (PC->GetHitResultUnderCursor(BuildTraceChannel, false, HitResult))
		{
			// 기본 위치: 충돌 지점
			float VerticalOffset = 0.0f;
			FVector NewLoc = HitResult.Location;
			NewLoc.Z += VerticalOffset;

			// 통나무 위에 쌓는 로직: 
			// 충돌한 면의 법선 방향(Normal)으로 통나무 절반 높이만큼 띄워주면 딱 붙습니다.
			// 예: 통나무 두께가 20이라면
			

			m_SnapLocation = NewLoc.GridSnap(5.0f); // 설정하신 그리드 적용
		}
	}
}

void UBuildComponent::SpawnPreviewActor()
{
	if (GetWorld() && m_CurrentBuildClass)
	{
		// 2. 만약 기존에 남은 프리뷰가 있다면 제거 (안전장치)
		if (!m_CurrentPreview)
		{
			// 3. 스폰 설정 (부딪히지 않아도 일단 스폰되도록 설정)
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			// 4. 현재 스냅 위치에 액터 스폰
			m_CurrentPreview = GetWorld()->SpawnActor<ABuildPreview>(m_CurrentBuildPreviewClass, m_SnapLocation, FRotator::ZeroRotator, SpawnParams);
			
		}

		

		

		if (m_CurrentPreview)
		{
			
			//m_CurrentPreview->SetActorScale3D(FVector(0.98f));
			//m_CurrentPreview->MeshComponent->SetRelativeScale3D(FVector(0.9f));
			// 2. 설치할 클래스의 '기본 메쉬' 정보 가져오기
		// *주의: 설치할 클래스(AActor)가 StaticMeshComponent를 가지고 있다고 가정합니다.
			AActor* DefaultActor = m_CurrentBuildClass->GetDefaultObject<AActor>();
			UStaticMeshComponent* DefaultMeshComp = DefaultActor->FindComponentByClass<UStaticMeshComponent>();

			if (DefaultMeshComp && DefaultMeshComp->GetStaticMesh())
			{
				// 미리 헤더에 선언해둔 프리뷰용 투명 머티리얼(m_PreviewMaterial)을 전달
				m_CurrentPreview->UpdatePreviewAppearance(DefaultMeshComp->GetStaticMesh(), m_PreviewMaterial);
			}

			// 3. 충돌 설정 (작성하신 코드 유지)
			TArray<UPrimitiveComponent*> Components;
			m_CurrentPreview->GetComponents<UPrimitiveComponent>(Components);
			for (UPrimitiveComponent* Comp : Components)
			{
				Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				Comp->SetGenerateOverlapEvents(true); // 반드시 true여야 함

				// 1. 레이캐스트는 통과
				Comp->SetCollisionResponseToChannel(BuildTraceChannel, ECR_Ignore);

				// 2. [중요] 다른 모든 오브젝트 채널에 대해 Overlap으로 설정하여 겹침을 감지하게 함
				// (설치된 통나무들이 WorldStatic이나 WorldDynamic이라면 그에 맞춰 설정)
				Comp->SetCollisionResponseToAllChannels(ECR_Overlap);

				// 다시 한번 레이 채널만 Ignore로 덮어쓰기
				Comp->SetCollisionResponseToChannel(BuildTraceChannel, ECR_Ignore);
				Comp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

// Sets default values for this component's properties
UBuildComponent::UBuildComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBuildComponent::SetBuildMode(bool curmode)
{
	m_bisbuildmode = curmode;
}

void UBuildComponent::SetSnapLocation(FVector vector)
{
	m_SnapLocation = vector;
}

void UBuildComponent::SetCurrentBuildClass(TSubclassOf<AActor> actor)
{
	m_CurrentBuildClass = actor;
}

void UBuildComponent::SetCurrentBuildPreviewClass(TSubclassOf<AActor> actor)
{
	m_CurrentBuildPreviewClass = actor;
}

void UBuildComponent::SetCurrentPreview(ABuildPreview* actor)
{
	m_CurrentPreview = actor;
}

void UBuildComponent::SetCurrentPreViewNum(int num)
{
	m_CurrentPreviewNum = num;
}

bool UBuildComponent::GetBuildMode()
{
	return m_bisbuildmode;
}

FVector UBuildComponent::GetSnapLocation()
{
	return m_SnapLocation;
}

TSubclassOf<AActor> UBuildComponent::GetCurrentBuildClass()
{
	return m_CurrentBuildClass;
}

ABuildPreview* UBuildComponent::GetCurrentPreview()
{
	return m_CurrentPreview;
}

int UBuildComponent::GetCurrentPreviewNum()
{
	return m_CurrentPreviewNum;
}

FName UBuildComponent::GetBuildItemID()
{
	return m_BuildItemName;
}

void UBuildComponent::SetBuildItemID(FName ItemID)
{
	m_BuildItemName = ItemID;
	BuildMode();
}


// Called when the game starts
void UBuildComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (m_bisbuildmode && m_CurrentPreview)
	{
		UpdateMouseSnapLocation();
		if (m_CurrentPreview)
		{
			//m_CurrentPreview->SetActorLocation(m_SnapLocation);
			// 현재 위치
			FVector CurrentLocation = m_CurrentPreview->GetActorLocation();

			// 목표 위치 (스냅된 마우스 좌표)
			FVector TargetLocation = m_SnapLocation;

			// 보간된 새로운 위치 계산 (속도 15.0f는 기호에 맞게 조절하세요)
			FVector SmoothedLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 15.0f);

			// 최종 위치 적용
			m_CurrentPreview->SetActorLocation(SmoothedLocation);
		}

		// 2. 설치 가능 여부 체크
		bool bIsOverlapping = false;
		TArray<AActor*> OverlappingActors;

		// 프리뷰 액터의 메쉬 컴포넌트를 기준으로 겹치는 액터가 있는지 확인
		// (프리뷰 자신은 무시하도록 설정되어 있어야 함)
		m_CurrentPreview->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			// 실제 설치된 구조물이나 환경 오브젝트와 겹치는지 확인
			if (Actor && Actor != m_CurrentPreview)
			{
				bIsOverlapping = true;
				break;
			}
		}

		// 3. 시각화 업데이트
		// 겹치는 게 있으면(true) 설치 불가(false)
		m_bCanPlace = !bIsOverlapping; // 겹치지 않아야 설치 가능
		m_CurrentPreview->SetCanBuild(!bIsOverlapping);
	}

	// ...
}

void UBuildComponent::BuildMode()
{
	if (m_bisbuildmode)
	{
		SetBuildMode(false);
		if (m_CurrentPreview)
		{
			m_CurrentPreview->Destroy();
			m_CurrentPreview = nullptr;
			//m_CurrentBuildPreviewClass = nullptr;
		}
	}
	else
	{
		SetBuildMode(true);
		SpawnPreviewActor();
		//SetCurrentBuildPreviewClass(m_CurrentBuildPreviewDefaultClass);
	}
}

void UBuildComponent::CompletePlacement()
{
	/*if (!m_bCanPlace)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot build here!"));
		return;
	}*/

	if (!GetWorld() || !m_CurrentBuildClass || !m_CurrentPreview) return;

	

	// 1. 프리뷰가 있던 '현재 위치'와 '회전값'을 가져옵니다.
	FVector SpawnLocation = m_CurrentPreview->GetActorLocation();
	FRotator SpawnRotation = m_CurrentPreview->GetActorRotation();

	// 2. 실제 액터(진짜 통나무)를 해당 위치에 스폰합니다.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* DeployedActor = GetWorld()->SpawnActor<AActor>(
		m_CurrentBuildClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);

	// 3. 배치가 성공했다면 프리뷰를 제거하고 참조를 비웁니다.
	if (DeployedActor)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("progress!"), false);
		// 2. 모드를 먼저 꺼서 Tick 로직이 더 이상 실행되지 않게 합니다.
		m_bisbuildmode = false;
		m_CurrentPreview->Destroy();
		m_CurrentPreview = nullptr;

		// 4. 빌드 모드를 계속 유지할지 아니면 끌지 결정합니다.
		// 연속해서 짓고 싶다면 여기서 m_bisbuildmode를 끄지 말고 바로 SpawnPreviewActor()를 호출하세요.
		
		
	}
}

