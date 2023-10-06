// Fill out your copyright notice in the Description page of Project Settings.


#include "Noun.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"
#include "CharactorAttributeSet.h"

// Sets default values
ANoun::ANoun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UCharactorAttributeSet>(TEXT("AttributeSet"));

}

// Called when the game starts or when spawned
void ANoun::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent != nullptr)
	{
		//��ʼ������
		if (PreloadedAbilities.Num() > 0)
		{
			for (auto i = 0; i < PreloadedAbilities.Num(); i++)
			{
				if (PreloadedAbilities[i] != nullptr)
				{
					// FGameplayAbilitySpec��GA��ʵ�����乹�캯���ĵڶ�����������GA�ĵȼ�������������ȫ��Ϊ1
					AbilitySystemComponent->GiveAbility(
						FGameplayAbilitySpec(PreloadedAbilities[i].GetDefaultObject(), 1));
				}
			}
		}

		//��ʼ��ASC
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}


// Called every frame
void ANoun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ANoun::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ANoun::GiveAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level /*= 1*/)
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
