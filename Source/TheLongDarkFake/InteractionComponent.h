// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableInterface.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELONGDARKFAKE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	bool m_bIsInteract = false;
	// 트레이스 설정 변수
	UPROPERTY(EditDefaultsOnly, Category = "Trace Settings")
	float TraceDistance = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Settings")
	float TraceRadius = 100.0f;

	// 현재 감지된 상호작용 가능한 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> DetectedActor; // TWeakObjectPtr를 사용하여 액터가 파괴되어도 안전하게 관리

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Detect();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RequestInteraction();
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* GetDetectedInteractableActor() const { return DetectedActor.Get(); }
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void setIsinteract(bool curinteract);
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool GetIsinteract();
};
