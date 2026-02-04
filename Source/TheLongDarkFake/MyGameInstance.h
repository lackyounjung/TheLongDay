// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MySaveGame.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category = "SaveLoad")
    bool bShouldLoadGame;
    // 슬롯 이름 정의
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveSystem")
    FString SaveSlotName = TEXT("SurvivalSlot_0");

    // 저장 실행 함수
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void SaveGame();

    // 로드 실행 함수
    UFUNCTION(BlueprintCallable, Category = "SaveSystem")
    void LoadGame();
	
};
