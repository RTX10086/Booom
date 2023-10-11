// Copyright Epic Games, Inc. All Rights Reserved.

#include "BooomCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ABooomCharacter

ABooomCharacter::ABooomCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	// 在OwnerActor的构造方法中创建的AttributeSet将会自动注册到ASC
	AttributeSet = CreateDefaultSubobject<UCharactorAttributeSet>(TEXT("AttributeSet"));	
	
	
}


void ABooomCharacter::BeginPlay()
{
	
	// Call the base class  
	Super::BeginPlay();
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//GAS
	if (AbilitySystemComponent != nullptr)
	{
		//初始化技能
		if (PreloadedAbilities.Num() > 0)
		{
			for (auto i = 0; i < PreloadedAbilities.Num(); i++)
			{
				if (PreloadedAbilities[i] != nullptr)
				{
					// FGameplayAbilitySpec是GA的实例，其构造函数的第二个参数代表GA的等级，这里暂令其全部为1
					AbilitySystemComponent->GiveAbility(
						FGameplayAbilitySpec(PreloadedAbilities[i].GetDefaultObject(), 1));
				}
			}
		}

		//初始化ASC
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABooomCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	APlayerController* pc = Cast<APlayerController>(Controller);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABooomCharacter::Move);

	}

}

void ABooomCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		AddMovementInput(this->GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(this->GetActorRightVector(), MovementVector.X);
	}
}

void ABooomCharacter::Look(const  FInputActionValue& Value, APlayerController* pc)
{

}

UAbilitySystemComponent* ABooomCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ABooomCharacter::GiveAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level /*= 1*/)
{

	if (AbilitySystemComponent)
	{
		if (HasAuthority() && Ability)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, Level));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

}

void ABooomCharacter::OnHealthChanged()
{
	K2_OnHealthChanged();
}

FVector2D ABooomCharacter::GetCursorPosion(class APlayerController* pc)
{
	FVector2D MousePosition; 
	ULocalPlayer* LocalPlayer = pc->GetLocalPlayer();
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			FVector MouseLocation = FVector(0,0,0);
			FVector MouseDirection = FVector(0, 0, 0);
			//获取鼠标位置
			pc->DeprojectMousePositionToWorld( MouseLocation, MouseDirection);
			//获取当前位置到目标位置所需要旋转的角度
			//只在XY平面旋转
			FVector TargetLoaction = FVector(MouseLocation.X, MouseLocation.Y, GetActorLocation().Z);
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoaction);
			SetActorRotation(Rotator);
			UE_LOG(LogTemp, Warning, TEXT("Cursor.position X :  %f , Y : %f"), GetActorLocation().X, GetActorLocation().Y);
			UE_LOG(LogTemp, Warning, TEXT("Cursor.position X :  %f , Y : %f"), MouseLocation.X, MouseLocation.Y);		
		}
	}
	return MousePosition;
}

void ABooomCharacter::OnPhysicalChanged()
{
	K2_OnPhysicalChanged();
}



