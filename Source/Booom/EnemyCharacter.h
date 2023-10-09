// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "CharactorAttributeSet.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class BOOOM_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities")
	class UAbilitySystemComponent* AbilitySystemComponent;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
    TArray<TSubclassOf<class UGameplayAbility>> PreloadedAbilities;

	UFUNCTION(BlueprintCallable, Category = "Ability System")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability, int32 Level = 1);
	
	UPROPERTY()
	UCharactorAttributeSet* AttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
