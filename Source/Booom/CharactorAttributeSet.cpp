// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactorAttributeSet.h"
#include "BooomCharacter.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

// Fill out your copyright notice in the Description page of Project Settings.
UCharactorAttributeSet::UCharactorAttributeSet():
	Health(100.0f),
	MaxHealth(100.0f),
	Physical(100.0f),
	MaxPhysical(100.0f)
{

}

void UCharactorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharactorAttributeSet, Health);
	DOREPLIFETIME(UCharactorAttributeSet, MaxHealth);
	DOREPLIFETIME(UCharactorAttributeSet, Physical);
	DOREPLIFETIME(UCharactorAttributeSet, MaxPhysical);
}

void UCharactorAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, Health, OldValue);
}

void UCharactorAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, MaxHealth, OldValue);
}

void UCharactorAttributeSet::OnRep_Physical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, Physical, OldValue);
}

void UCharactorAttributeSet::OnRep_MaxPhysical(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, MaxPhysical, OldValue);
}

void UCharactorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetPhysicalAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxPhysical());
	}
}

void UCharactorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// 仅在instant GameplayEffect使Attribute的 BaseValue改变时触发。
	Super::PostGameplayEffectExecute(Data);

	ABooomCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		AActor* TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetCharacter = Cast<ABooomCharacter>(TargetActor);
	}
	if (nullptr != TargetCharacter)
	{
		if (Data.EvaluatedData.Attribute == GetHealthAttribute())
		{
			TargetCharacter->OnHealthChanged();
		}
		if (Data.EvaluatedData.Attribute == GetPhysicalAttribute())
		{
			TargetCharacter->OnPhysicalChanged();
		}
	}

}
