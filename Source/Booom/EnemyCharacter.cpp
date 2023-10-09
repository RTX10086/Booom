// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "CharactorAttributeSet.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UCharactorAttributeSet>(TEXT("AttributeSet"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
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

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemyCharacter::GiveAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level /*= 1*/)
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