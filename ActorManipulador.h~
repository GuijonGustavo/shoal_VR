/*=================================================
* FileName: ActorManipulador.h
* 
* Created by: Gustavo Magallanes Guijón
* Project name: Cardumen
* Unreal Engine version: 4.19.1
* Created on: 2018/04/17
*
* =================================================*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorManipulador.generated.h"


UCLASS()
class AActorManipulador : public AActor
{
	GENERATED_BODY()

	
public:

	// Type of flocks to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TArray<UClass*> flockTypes;

	// # of fish in respective flock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TArray<float> numInFlock;

	// min Z of volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float minZ = -9000.0f;

	// max Z of volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float maxZ = -950.0f;
	
	// Length of underwater box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float underwaterBoxLength = 10000;

	// Attach fish manager to player?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool attachToPlayer = false;

	// Player Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool DebugMode = false;

	// Player Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UClass* playerType;

	// Player
	AActor* player;

	// get min z
	float getMinZ();

	// get min z
	float getMaxZ();

	/** Constructor */
	AFishManager(const FObjectInitializer& ObjectInitializer);

protected:

	/** Overrided tick function */
	virtual void Tick(float val) override;

	/** setup the fish manager */
	void setup();

	/** move fish manager to player */
	void moveToPlayer();

	/* max bounds of water*/
	float maxX;
	float maxY;
	float minX;
	float minY;

	// is set up?
	bool isSetup = false;

	// Are fish spawned?
	bool areFishSpawned = false;
};
