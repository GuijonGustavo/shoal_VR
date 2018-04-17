/*=================================================
* FileName: ActorManipulador.cpp
* 
* Created by: Gustavo Magallanes Guijón
* Project name: Cardumen
* Unreal Engine version: 4.19.1
* Created on: 2018/04/17
*
* =================================================*/

#include "ActorManipulador.h"
#include "PawnPez.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Classes/Engine/World.h"


AActorManipulador::AActorManipulador(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AActorManipulador::Tick(float val)
{
	setup();

	if (attachToPlayer)
	{
		moveToPlayer();
	}
}

void AActorManipulador::moveToPlayer()
{
	if (player)
		this->SetActorLocation(player->GetActorLocation());
}

float AActorManipulador::getMinZ()
{
	return minZ;
}

float AActorManipulador::getMaxZ()
{
	return maxZ;
}

void AActorManipulador::setup()
{
	if (isSetup == false){
		if (!areFishSpawned)
		{
			maxX = GetActorLocation().X + underwaterBoxLength;
			maxY = GetActorLocation().Y + underwaterBoxLength;
			minX = GetActorLocation().X - underwaterBoxLength;
			minY = GetActorLocation().Y - underwaterBoxLength;

			UWorld* const world = GetWorld();
			int numFlocks = flockTypes.Num();
			for (int i = 0; i < numFlocks; i++)
			{
				FVector spawnLoc = FVector(FMath::FRandRange(minX, maxX), FMath::FRandRange(minY, maxY), FMath::FRandRange(minZ, maxZ));
				AFlockFish *leaderFish = NULL;
				for (int j = 0; j < numInFlock[i]; j++)
				{
					AFlockFish *aFish = Cast<AFlockFish>(world->SpawnActor(flockTypes[i]));
					aFish->isLeader = false;
					aFish->DebugMode = DebugMode;
					aFish->underwaterMax = FVector(maxX, maxY, maxZ);
					aFish->underwaterMin = FVector(minX, minY, minZ);
					aFish->underwaterBoxLength = underwaterBoxLength;
					spawnLoc = FVector(FMath::FRandRange(minX, maxX), FMath::FRandRange(minY, maxY), FMath::FRandRange(minZ, maxZ));
					if (j == 0)
					{
						aFish->isLeader = true;
						leaderFish = aFish;
					}
					else if (leaderFish != NULL)
					{	
						aFish->leader = leaderFish;
					}
					aFish->SetActorLocation(spawnLoc);
				}
			}
			areFishSpawned = true;
		}

		if (attachToPlayer)
		{
			TArray<AActor*> aPlayerList;
			UGameplayStatics::GetAllActorsOfClass(this, playerType, aPlayerList);
			if (aPlayerList.Num() > 0)
			{	
				player = aPlayerList[0];
				isSetup = true;
			}
		} 
		else
		{
			isSetup = true;
		}

	}
}
