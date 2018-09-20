/*=================================================
* FileName: PezState.cpp
* 
* Created by: Gustavo Magallanes Guijón
* Project name: Cardumen
* Unreal Engine version: 4.19.1
* Created on: 2018/04/17
*
* =================================================*/

#include "PezState.h"
#include "Pez.h"

void SeekState::Update(float delta)
{	
	Pez->isFleeing = false;
	Pez->fleeTarget = NULL;
	Pez->preyTarget = NULL;

	if (Pez->isLeader)
	{
		SeekTarget(delta);
	}
	else
	{
		Flock(delta);
	}
}

void SeekState::SeekTarget(float delta)
{	
	Pez->curSpeed = FMath::Lerp(Pez->curSpeed, Pez->speed, delta * Pez->SeekDecelerationMultiplier);

	FVector targetRotation = (Pez->getSeekTarget() - Pez->GetActorLocation() + Pez->AvoidObstacle());
	FRotator leaderRotation = FRotationMatrix::MakeFromX(targetRotation).Rotator();
	leaderRotation = FMath::RInterpTo(Pez->GetActorRotation(), leaderRotation, delta, Pez->turnSpeed);
	Pez->setRotation(leaderRotation);

	FVector leaderVelocity = Pez->GetActorForwardVector() * (delta * Pez->curSpeed);
	Pez->setVelocity(leaderVelocity);
}

void SeekState::Flock(float delta)
{
	FVector seperation = FVector(0, 0, 0);
	if (Pez->nearbyFriends.IsValidIndex(0))
	{
		TArray<AActor*> neighborList = Pez->nearbyFriends;
		int neighborCount = 0;
		for (int i = 0; i < Pez->nearbyFriends.Num(); i++)
		{
			if (neighborList.IsValidIndex(i))
			{
				seperation += neighborList[i]->GetActorLocation() - Pez->GetActorLocation();
				neighborCount++;
			}

			if (i == Pez->NumNeighborsToEvaluate && i != 0)
			{
				break;
			}
		}
		seperation = ((seperation / neighborCount) * -1);
		seperation.Normalize();
		seperation *= Pez->neighborSeperation;
	}

	FVector distBehind = (Cast<APez>(Pez->leader)->getVelocity() * -1);
	distBehind.Normalize();
	distBehind *= Pez->followDist;

	FVector leaderLocation = Pez->leader->GetActorLocation();
	FVector flockerVelocity = distBehind + leaderLocation + seperation + Pez->AvoidObstacle();
	FRotator flockerRotation = FRotationMatrix::MakeFromX(flockerVelocity - Pez->GetActorLocation()).Rotator();

	float newSpeed = Pez->speed;
	if (Pez->GetDistanceTo(Pez->leader) > Pez->distBehindSpeedUpRange)
	{
		Pez->curSpeed = FMath::Lerp(Pez->curSpeed, Pez->maxSpeed, delta);
	}
	else
	{
		Pez->curSpeed = FMath::Lerp(Pez->curSpeed, Pez->speed, delta);
	}


	FVector flockVelocity = Pez->GetActorForwardVector() * (delta * Pez->curSpeed);
	Pez->setVelocity(flockVelocity);

	FRotator flockRotation = FMath::RInterpTo(Pez->GetActorRotation(), flockerRotation, delta, Pez->turnSpeed);
	Pez->setRotation(flockRotation);
}


void FleeState::Update(float delta)
{
	Pez->isFleeing = true;
	Pez->fleeTarget = Enemy;
	Pez->preyTarget = NULL;
	if (Pez->GetDistanceTo(Enemy) >= Pez->fleeDistance)
	{
		Pez->setState(new SeekState(Pez));
	}
	FleeFromEnemy(delta);
}

void FleeState::FleeFromEnemy(float delta)
{
	Pez->curSpeed = FMath::Lerp(Pez->curSpeed, Pez->maxSpeed, (delta * Pez->FleeAccelerationMultiplier));
	
	FVector fleeVelocity = Pez->GetActorForwardVector() * (delta * Pez->curSpeed);
	Pez->setVelocity(fleeVelocity);

	FVector targetRotation = (Pez->GetActorLocation() - Enemy->GetActorLocation()) + Pez->AvoidObstacle();
	FRotator fleeRotation = FRotationMatrix::MakeFromX(targetRotation).Rotator();
	fleeRotation = FMath::RInterpTo(Pez->GetActorRotation(), fleeRotation, delta, Pez->turnSpeed);
	Pez->setRotation(fleeRotation);

}


void ChaseState::Update(float delta)
{
	Pez->isFleeing = false;
	Pez->fleeTarget = NULL;
	Pez->preyTarget = Prey;
	EatPrey();
	ChasePrey(delta);
}

void ChaseState::EatPrey()
{
	if (Pez->GetDistanceTo(Prey) < 1000)
	{
		float zLoc = Pez->minZ + FMath::Abs(0.25 * Pez->maxZ);
		Prey->SetActorLocation(FVector(FMath::FRandRange(Pez->minX, Pez->maxX), FMath::FRandRange(Pez->minY, Pez->maxX), zLoc));
		Pez->isFull = true;
		Pez->setState(new SeekState(Pez));
	}
}

void ChaseState::ChasePrey(float delta)
{
	Pez->curSpeed = FMath::Lerp(Pez->curSpeed, Pez->maxSpeed, (delta * Pez->ChaseAccelerationMultiplier));

	FVector chaseVelocity = Pez->GetActorForwardVector() * (delta * Pez->curSpeed);
	Pez->setVelocity(chaseVelocity);

	FVector seperation = FVector(0, 0, 0);
	if (Pez->nearbyFriends.IsValidIndex(0))
	{
		int neighborCount = 0;
		TArray<AActor*> neighborList = Pez->nearbyFriends;
		for (int i = 0; i < Pez->NumNeighborsToEvaluate; i++)
		{
			if (neighborList.IsValidIndex(i))
			{
				seperation += neighborList[i]->GetActorLocation() - Pez->GetActorLocation();
				neighborCount++;
			}
		}
		seperation = ((seperation / neighborCount) * -1);
		seperation.Normalize();
		seperation *= Pez->neighborSeperation;
	}

	FVector preyLocation = Prey->GetActorLocation();
	FVector flockerVelocity = ((preyLocation + seperation) - Pez->GetActorLocation()) + Pez->AvoidObstacle();
	FRotator flockerRotation = FRotationMatrix::MakeFromX(flockerVelocity).Rotator();
	FRotator chaseRotation = FMath::RInterpTo(Pez->GetActorRotation(), flockerRotation, delta, Pez->turnSpeed);
	Pez->setRotation(chaseRotation);
}
