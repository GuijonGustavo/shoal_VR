/*=================================================
* FileName: Pez.h
* 
* Created by: Gustavo Magallanes Guijón
* Project name: Cardumen
* Unreal Engine version: 4.19.1
* Created on: 2017/17/18
* =================================================*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Manejador.h"
#include "Pez.generated.h"


UCLASS()
class APez : public APawn
{
	GENERATED_BODY()

	class PezState* currentState;

	class USkeletalMeshComponent* base;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* FishInteractionSphere;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool isLeader;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<UClass*> enemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<UClass*> preyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UClass* neighborType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float followDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float turnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float turnFrequency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float hungerResetTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float distBehindSpeedUpRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float SeperationDistanceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float FleeDistanceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float FleeAccelerationMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float ChaseAccelerationMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float SeekDecelerationMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float AvoidForceMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float AvoidanceForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	UClass* playerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FVector underwaterMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	FVector underwaterMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float CustomZSeekMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float CustomZSeekMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float NumNeighborsToEvaluate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	float UpdateEveryTick;

	FVector target;

	float InteractionSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	bool DebugMode;

	float fleeDistance;

	float neighborSeperation;

	float curSpeed;

	AActor *leader;

	TArray<AActor*> prey;

	TArray<AActor*> enemies;

	TArray<AActor*> neighbors;

	TArray<AActor*> nearbyEnemies;

	TArray<AActor*> nearbyPrey;

	TArray<AActor*> nearbyFriends;

	AActor *fleeTarget;

	AActor *preyTarget;

	bool isFleeing;

	bool isFull;

	float limitesPecera;

	FVector AvoidObstacle();

	float AvoidanceDistance;

	float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;

	APez(const FObjectInitializer& ObjectInitializer);

	FVector getSeekTarget()
	{
		return target;
	}

	void setState(class PezState* newState)
	{
		currentState = newState;
	}

	void setRotation(FRotator newRotation)
	{
		curRotation = newRotation;
	};

	void setVelocity(FVector newVelocity)
	{
		curVelocity = newVelocity;
	};

	FRotator getRotation()
	{
		return curRotation;
	};

	FVector getVelocity()
	{
		return curVelocity;
	};

protected:
	virtual void Tick(float delta) override;

	void Setup();

	void ManageTimers(float delta);

	void ChooseState();

	void MoveBounds(float delta);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	void UpdateState(float delta);

	void spawnTarget();

	void Debug();

	FVector curVelocity;

	FRotator curRotation;

	float turnTimer;

	bool isSetup;

	float hungerTimer;

	float updateTimer;

	bool hasManejador;

	AActor* manejador;


};
