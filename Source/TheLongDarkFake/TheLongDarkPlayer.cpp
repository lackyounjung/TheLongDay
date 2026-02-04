// Fill out your copyright notice in the Description page of Project Settings.


#include "TheLongDarkPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATheLongDarkPlayer::ATheLongDarkPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*SurvivalStats = CreateDefaultSubobject<UPlayerSurvivalStatComponent>(TEXT("SurvivalStatsComponent"));*/

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = -50.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ATheLongDarkPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerCon = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerCon->GetLocalPlayer()))
		{
			subSys->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

void ATheLongDarkPlayer::Move(const FInputActionValue& Value)
{
	if (!m_bCanmove) return;
	if (GetWorld()->IsPaused()) return;
	const FVector2D MoveDir = Value.Get<FVector2D>();
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("movedir : %s"), *MoveDir.ToString()));
	if (Controller)
	{
		GetCharacterMovement()->MaxWalkSpeed = m_pSpd;
		const auto Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const auto ForwarDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const auto RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwarDir, MoveDir.Y);
		AddMovementInput(RightDir, MoveDir.X);

	}
}

void ATheLongDarkPlayer::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVec = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxisVec.X);
		AddControllerPitchInput(LookAxisVec.Y);
	}
}

void ATheLongDarkPlayer::JumpStart(const FInputActionValue& Value)
{
	if (!m_bCanJump) return;
	Jump();
}

void ATheLongDarkPlayer::JumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}

void ATheLongDarkPlayer::Aming(const FInputActionValue& Value)
{
	USkeletalMeshComponent* CharacterMesh = GetMesh();

	if (m_bCanAming)
	{
		// === 3인칭 뷰로 돌아가기 (Turning 1st Person OFF) ===
		SetCanAming(false);

		CharacterMesh->SetOwnerNoSee(false);

		// 1. 카메라 붐 길이 복원 (3인칭: 캐릭터에서 멀리)
		CameraBoom->TargetArmLength = 200.0f; // 기본 3인칭 거리 (원하는 값으로 설정)

		// 2. 컨트롤러 회전 사용 설정 복원
		// 카메라 붐의 Yaw만 컨트롤러를 따라가도록 설정 (3인칭 기본 설정)
		CameraBoom->bUsePawnControlRotation = true;

		// 3. 컨트롤러가 캐릭터의 회전을 따라가도록 설정 복원
		// 3인칭에서는 캐릭터가 이동 방향을 바라보도록 설정하는 경우가 많습니다.
		// GetCharacterMovement()->bOrientRotationToMovement = true; 

		// 4. (선택) Spring Arm의 상대 위치 복원 (오른쪽 어깨 뷰 등)
		CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));

		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("3rd Person ON"), false);
	}		
	else
	{
		// === 1인칭 뷰로 전환 (Turning 1st Person ON) ===
		SetCanAming(true);

		//CharacterMesh->SetOwnerNoSee(true);
		// 1. 카메라 붐 길이 단축 (1인칭: 캐릭터에 매우 가깝게)
		// 0.0f 또는 매우 작은 값 (1인칭 시야 확보)
		//CameraBoom->TargetArmLength = 100.0f;
		CameraBoom->TargetArmLength = 0.0f;

		// 2. 컨트롤러 회전 사용 설정
		// 1인칭 시에는 카메라 붐이 컨트롤러의 모든 회전을 완벽하게 따라가야 합니다.
		CameraBoom->bUsePawnControlRotation = true; // Spring Arm이 컨트롤러 회전을 따라감

		// 3. (필수) 컨트롤러가 캐릭터의 회전을 따라가지 않도록 설정
		// 1인칭에서는 시점(컨트롤러)만 회전하고 캐릭터 메시 자체는 그대로 둘 수 있습니다.
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// 4. (필수) 카메라 붐의 상대 위치를 캐릭터의 눈높이로 설정
		// 이 위치는 캐릭터의 눈(Mesh의 Head Socket) 위치와 비슷해야 합니다.
		// Character Mesh의 캡슐 컴포넌트(Root)를 기준으로 설정합니다.
		//CameraBoom->SetRelativeLocation(FVector(0.0f, 100.0f, 50.0f)); // Z축을 캐릭터 눈높이(예: 60cm)로 설정
		CameraBoom->SetRelativeLocation(FVector(30.0f, 0.0f, 50.0f)); // Z축을 캐릭터 눈높이(예: 60cm)로 설정

		// 5. (선택) 카메라의 FOV(화각) 변경
		// FollowCamera->SetFieldOfView(90.0f); // 1인칭에 적합한 FOV로 변경

		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, TEXT("1st Person ON"), false);
	}
	
}

void ATheLongDarkPlayer::Shoting(const FInputActionValue& Value)
{

}





// Called every frame
void ATheLongDarkPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATheLongDarkPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EInputCom = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EInputCom->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheLongDarkPlayer::Move);
		EInputCom->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheLongDarkPlayer::Look);
		EInputCom->BindAction(JumpAction, ETriggerEvent::Started, this, &ATheLongDarkPlayer::JumpStart);
		EInputCom->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATheLongDarkPlayer::JumpEnd);
		EInputCom->BindAction(AmingAction, ETriggerEvent::Triggered, this, &ATheLongDarkPlayer::Aming);
		EInputCom->BindAction(ShotingAction, ETriggerEvent::Triggered, this, &ATheLongDarkPlayer::Shoting);
	}

}

void ATheLongDarkPlayer::SetMovementSpeed(float newspeed)
{
	m_pSpd = newspeed;
}

float ATheLongDarkPlayer::GetMovementSpeed()
{
	return m_pSpd;
}

void ATheLongDarkPlayer::SetCanLock(bool canlock)
{
	m_bCanlock = canlock;
}

bool ATheLongDarkPlayer::GetCanLock()
{
	return m_bCanlock;
}

void ATheLongDarkPlayer::SetCanMove(bool canMove)
{
	m_bCanmove = canMove;
}

bool ATheLongDarkPlayer::GetCanMove()
{
	return m_bCanmove;
}

void ATheLongDarkPlayer::SetCanAttack(bool canattack)
{
	m_bCanAttack = canattack;
}

bool ATheLongDarkPlayer::GetCanAttack()
{
	return m_bCanAttack;
}

void ATheLongDarkPlayer::SetCanJump(bool canjump)
{
	m_bCanJump = canjump;
}

bool ATheLongDarkPlayer::GetCanJump()
{
	return m_bCanJump;
}

void ATheLongDarkPlayer::SetCanSprint(bool cansprint)
{
	m_bCanSprint = cansprint;
}

bool ATheLongDarkPlayer::GetCanSprint()
{
	return m_bCanSprint;
}

void ATheLongDarkPlayer::SetCanAming(bool canAming)
{
	m_bCanAming = canAming;
}

bool ATheLongDarkPlayer::GetCanAming()
{
	return m_bCanAming;
}

void ATheLongDarkPlayer::SetCanShoting(bool canShoting)
{
	m_bCanShoting = canShoting;
}

bool ATheLongDarkPlayer::GetCanShoting()
{
	return m_bCanShoting;
}

float ATheLongDarkPlayer::GetMoveSpeed()
{
	float Speed = GetCharacterMovement()->Velocity.SquaredLength() / (GetCharacterMovement()->MaxWalkSpeed * GetCharacterMovement()->MaxWalkSpeed);
	return FMath::Clamp(Speed, 0.f, 1.0f);
}

void ATheLongDarkPlayer::SetInteraction(bool Interaction)
{
	m_binteractionwithResource = Interaction;
}

bool ATheLongDarkPlayer::GetInteraction()
{
	return m_binteractionwithResource;
}

void ATheLongDarkPlayer::SetInteractionwithitem(bool Interaction)
{
	m_binteractionwithItem = Interaction;
}

bool ATheLongDarkPlayer::GetInteractionitem()
{
	return m_binteractionwithItem;
}

