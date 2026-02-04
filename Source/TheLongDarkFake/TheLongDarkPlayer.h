 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerSurvivalStatComponent.h" 
#include "TheLongDarkPlayer.generated.h"

UCLASS()
class THELONGDARKFAKE_API ATheLongDarkPlayer : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> AmingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShotingAction;

	// UPROPERTY로 선언하여 언리얼 엔진의 리플렉션 시스템에 등록합니다.
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Survival", meta = (AllowPrivateAccess = "true"))
	UPlayerSurvivalStatComponent* SurvivalStats;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	float m_pSpd = 300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanlock =true;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanmove = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanJump = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanSprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanAming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_bCanShoting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_binteractionwithResource = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "stat", meta = (AllowPrivateAccess = "true"))
	bool m_binteractionwithItem = false;

public:
	// Sets default values for this character's properties
	ATheLongDarkPlayer();
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpStart(const struct FInputActionValue& Value);
	void JumpEnd(const struct FInputActionValue& Value);
	void Aming(const struct FInputActionValue& Value);
	void Shoting(const struct FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetMovementSpeed(float newspeed);
	float GetMovementSpeed();

	void SetCanLock(bool canlock);
	bool GetCanLock();

	void SetCanMove(bool canMove);
	bool GetCanMove();

	void SetCanAttack(bool canattack);
	bool GetCanAttack();
	
	void SetCanJump(bool canjump);
	bool GetCanJump();

	void SetCanSprint(bool cansprint);
	bool GetCanSprint();

	void SetCanAming(bool canAming);
	bool GetCanAming();

	UFUNCTION(BlueprintCallable)
	void SetCanShoting(bool canShoting);
	bool GetCanShoting();

	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed();

	void SetInteraction(bool Interaction);
	UFUNCTION(BlueprintCallable)
	bool GetInteraction();

	void SetInteractionwithitem(bool Interaction);
	UFUNCTION(BlueprintCallable)
	bool GetInteractionitem();
};
