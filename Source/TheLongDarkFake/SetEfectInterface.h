// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SetEfectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USetEfectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THELONGDARKFAKE_API ISetEfectInterface
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//1. 효과 실행(데이터 주입 완료 후 단순 실행 명령)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EffectInterface")
	void ExecuteEffect();
	// 2. 효과 내부 정리 (DOT 타이머 중지, 스탯 복구 등)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "EffectInterface")
	void CleanUpEffect();
};
