// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API ULoadingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	float m_num = 0;
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable, Category = "MyBlueprintFunctions")
	bool setRoundPercent(float num);
	
	UFUNCTION(BlueprintCallable)
	bool  CallBlueprintFunction(float percent);
};
