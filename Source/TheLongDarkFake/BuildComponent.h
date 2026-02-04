// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildPreview.h"
#include "Components/ActorComponent.h"
#include "BuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UBuildComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	bool m_bCanPlace = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool m_bisbuildmode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector m_SnapLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName m_BuildItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_CurrentBuildClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_CurrentBuildPreviewClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> m_CurrentBuildPreviewDefaultClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ABuildPreview*  m_CurrentPreview;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* m_PreviewMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int m_CurrentPreviewNum;

	// 실시간으로 마우스 위치를 업데이트하는 함수
	void UpdateMouseSnapLocation();

	// 추적할 채널 (예: 바닥이나 특정 가구 레이어만 감지)
	UPROPERTY(EditAnywhere, Category = "Build Settings")
	TEnumAsByte<ECollisionChannel> BuildTraceChannel = ECC_Visibility;

	// 실제 스폰을 담당할 내부 함수
	void SpawnPreviewActor();

public:	
	// Sets default values for this component's properties
	UBuildComponent();
	UFUNCTION(BlueprintCallable)
	void SetBuildMode(bool curmode);
	UFUNCTION(BlueprintCallable)
	void SetSnapLocation(FVector vector);
	UFUNCTION(BlueprintCallable)
	void SetCurrentBuildClass(TSubclassOf<AActor> actor);
	UFUNCTION(BlueprintCallable)
	void SetCurrentBuildPreviewClass(TSubclassOf<AActor> actor);
	UFUNCTION(BlueprintCallable)
	void SetCurrentPreview(ABuildPreview* actor);
	UFUNCTION(BlueprintCallable)
	void SetCurrentPreViewNum(int num);
	UFUNCTION(BlueprintCallable)
	bool GetBuildMode();
	UFUNCTION(BlueprintCallable)
	FVector GetSnapLocation();
	UFUNCTION(BlueprintCallable)
	TSubclassOf<AActor> GetCurrentBuildClass();
	UFUNCTION(BlueprintCallable)
	ABuildPreview* GetCurrentPreview();
	UFUNCTION(BlueprintCallable)
	int GetCurrentPreviewNum();
	UFUNCTION(BlueprintCallable)
	FName GetBuildItemID();

	UFUNCTION(BlueprintCallable)
	void SetBuildItemID(FName ItemID);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void BuildMode();
	UFUNCTION(BlueprintCallable)
	void CompletePlacement();
		
};
