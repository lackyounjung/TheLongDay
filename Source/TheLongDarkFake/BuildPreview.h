// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildPreview.generated.h"

UCLASS()
class THELONGDARKFAKE_API ABuildPreview : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterial;
	bool m_LastCanBuildState = true; // 이전 상태 저장용
	
public:	
	// Sets default values for this actor's properties
	ABuildPreview();
	// 설치 가능 여부에 따라 색상을 변경하는 함수
	void SetCanBuild(bool bCanBuild);

	// 외형을 담당할 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	// 프리뷰의 외형과 머티리얼을 업데이트하는 함수
	void UpdatePreviewAppearance(UStaticMesh* NewMesh, UMaterialInterface* PreviewMaterial);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
