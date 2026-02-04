// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildPreview.h"

// Sets default values
ABuildPreview::ABuildPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	RootComponent = MeshComponent;

    // --- 콜리전 설정 추가 ---
    // 1. 쿼리 전용으로 설정 (물리적으로 부딪히진 않지만 겹침은 감지)
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    // 2. 모든 채널을 무시하도록 초기화
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

    // 3. 특정 채널(예: Static 물체나 이미 설치된 구조물 채널)에 대해서는 Overlap으로 설정
    // 여기서는 기본적으로 WorldStatic과 WorldDynamic에 겹침을 감지하도록 설정합니다.
    MeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
    MeshComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

    // 만약 전용 BuildTraceChannel이 있다면 그것도 Overlap으로 설정하세요.
    //MeshComponent->SetCollisionResponseToChannel(BuildTraceChannel, ECR_Overlap);

    // 4. 이 옵션이 켜져 있어야 Overlap 이벤트를 감지할 수 있습니다.
    MeshComponent->SetGenerateOverlapEvents(true);

}

void ABuildPreview::SetCanBuild(bool bCanBuild)
{
    // 이전 상태와 다를 때만 머티리얼 업데이트 실행
    if (DynamicMaterial && (m_LastCanBuildState != bCanBuild))
    {
        m_LastCanBuildState = bCanBuild;

        FLinearColor Color = bCanBuild ? FLinearColor(0.0f, 0.5f, 1.0f, 1.0f) : FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
        DynamicMaterial->SetVectorParameterValue(TEXT("TintColor"), Color);

        // 디버깅용 로그: 실제로 색이 바뀌는 시점에만 찍힙니다.
        UE_LOG(LogTemp, Warning, TEXT("Preview Color Changed: %s"), bCanBuild ? TEXT("Blue") : TEXT("Red"));
    }
}



void ABuildPreview::UpdatePreviewAppearance(UStaticMesh* NewMesh, UMaterialInterface* PreviewMaterial)
{
    if (MeshComponent && NewMesh)
    {
        MeshComponent->SetStaticMesh(NewMesh);
        // 다이내믹 머티리얼 생성 (한 번만)
        DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, PreviewMaterial);
        if (DynamicMaterial)
        {
            // 모든 슬롯에 투명 머티리얼 적용
            int32 MaterialCount = MeshComponent->GetNumMaterials();
            for (int32 i = 0; i < MaterialCount; ++i)
            {
                MeshComponent->SetMaterial(i, DynamicMaterial);
            }

            SetCanBuild(true);
        }
    }
}

// Called when the game starts or when spawned
void ABuildPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

