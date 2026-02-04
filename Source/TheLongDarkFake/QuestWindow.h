// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestWindow.generated.h"

/**
 * 
 */
UCLASS()
class THELONGDARKFAKE_API UQuestWindow : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	class UQuestComponent* QuestComp;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ActiveQuestScrollBox;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* CompletedQuestScrollBox;
	UPROPERTY(EditAnywhere, Category = "Quest UI")
	TSubclassOf<class UUserWidget> QuestSlotClass;
public:
	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void RefreshQuestList();
	void NativeConstruct();
	UFUNCTION()
	void HandleQuestUpdated(const FQuestStatus& QuestStatus);

	UFUNCTION()
	void HandleQuestCompleted(FName QuestID);
	
	
};
