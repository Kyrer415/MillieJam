// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyWorldSubsystem.generated.h"

class AMySectionalDoor;
UCLASS()
class KYRER_MILLIEJAM_API UMyWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	


private:

	TMap<int32, AMySectionalDoor*> SectionDoors;
	TSet<int32> ObtainedKeys;

public:

	void RegisterSectionDoor(int32 DoorID, AMySectionalDoor* Door);

	UFUNCTION(BlueprintCallable, Category = "Subsyste, Settings")
	void GiveKey(int32 KeyID);

};
