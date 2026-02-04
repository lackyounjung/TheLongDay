// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SetPopupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USetPopupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THELONGDARKFAKE_API ISetPopupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 팝업의 확인 버튼이 눌렸을 때 실행될 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnPopupConfirmed();
};
