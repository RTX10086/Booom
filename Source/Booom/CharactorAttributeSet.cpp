// Fill out your copyright notice in the Description page of Project Settings.


#include "CharactorAttributeSet.h"
#include "BooomCharacter.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

// Fill out your copyright notice in the Description page of Project Settings.
UCharactorAttributeSet::UCharactorAttributeSet():
	Health(100.0f),
	MaxHealth(100.0f),
	Physical(100.0f),
	MaxPhysical(100.0f),
	Speed(500.0f),
	MaxSpeed(2000.0f)
{

}

void UCharactorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCharactorAttributeSet, Health);
	DOREPLIFETIME(UCharactorAttributeSet, MaxHealth);
	DOREPLIFETIME(UCharactorAttributeSet, Physical);
	DOREPLIFETIME(UCharactorAttributeSet, MaxPhysical);

	DOREPLIFETIME(UCharactorAttributeSet, Speed);
	DOREPLIFETIME(UCharactorAttributeSet, MaxSpeed);
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

void UCharactorAttributeSet::OnRep_Speed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, Speed, OldValue);
}

void UCharactorAttributeSet::OnRep_MaxSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharactorAttributeSet, MaxSpeed, OldValue);
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
/*Running D:/2/Epic Games/UE_5.3/Engine/Build/BatchFiles/Build.bat  -projectfiles -project="D:/UE/Booom-master/Booom-master/Booom.uproject"
-game -rocket -progress -log="D:\UE\Booom-master\Booom-master/Saved/Logs/UnrealVersionSelector-2023.10.06-22.29.53.log"
Using bundled DotNet SDK version: 6.0.302
Running UnrealBuildTool: dotnet "..\..\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" -projectfiles -project="D:/UE/Booom-master/Booom-master/Booom.uproject" -game -rocket -progress -log="D:\UE\Booom-master\Booom-master/Saved/Logs/UnrealVersionSelector-2023.10.06-22.29.53.log"
Log file: D:\UE\Booom-master\Booom-master\Saved\Logs\UnrealVersionSelector-2023.10.06-22.29.53.log
Log file: C:\Users\as\AppData\Local\UnrealBuildTool\Log_GPF.txt

Some Platforms were skipped due to invalid SDK setup: IOS, Android, Linux, LinuxArm64.
See the log file for detailed information


Generating VisualStudio project files:
Discovering modules, targets and source code for project...
Visual Studio 2019 does not support .NET 6.0 C# projects, these projects will not be added to the generated solution.
Please generate the Visual Studio 2022 solution if .NET 6.0 C# project support is required.
Microsoft platform targets must be compiled with Visual Studio 2022 17.4 (MSVC 14.34.x) or later for the installed engine. Please update Visual Studio 2022 and ensure no configuration is forcing WindowsTargetRules.Compiler to VisualStudio2019. The current compiler version was detected as: 14.29.30148
*/