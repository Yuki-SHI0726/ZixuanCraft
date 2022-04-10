// Fill out your copyright notice in the Description page of Project Settings.
// SpawnableCharacter.h
// Author: Zixuan Shi
// NPC character class used in the game. All NPC should be inherited from this

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpawnableCharacter.generated.h"

UENUM(BlueprintType)
enum class EAgentState : uint8 
{
	Idle		UMETA(DisplayName = "Idle"),
	Engaged 	UMETA(DisplayName = "Engaged"),
	Dead		UMETA(DisplayName = "Dead"),
};

/**
* Base character class for all spawnable objects in the game
*/
UCLASS()
class ZIXUANCRAFT_API ASpawnableCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	/** State used for AI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAgentState AgentState = EAgentState::Idle;

	/** Current & Max health */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Health = 30.0f;

	/** How long to destroy the mesh after death. Used for playing the death animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float DeathLifeSpan = 2.0f;

public:
	ASpawnableCharacter();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack(AActor* Target);
};
