// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWorldSubsystem.h"
#include "MySectionalDoor.h"

void UMyWorldSubsystem::RegisterSectionDoor(
    int32 DoorID,
    AMySectionalDoor* Door)
{
    if (!Door)
        return;

    SectionDoors.Add(DoorID, Door);
}

void UMyWorldSubsystem::GiveKey(int32 KeyID)
{

#if !UE_BUILD_SHIPPING
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ВЫЗОВ GiveKey!"));
#endif

    ObtainedKeys.Add(KeyID);

    for (const auto& Pair : SectionDoors)
    {
        AMySectionalDoor* Door = Pair.Value;

#if !UE_BUILD_SHIPPING
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ПЕРЕБОР!"));
#endif

        if (!Door)
            continue;
        if (Door->RequiredKeyID == KeyID)
        {
#if !UE_BUILD_SHIPPING
            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("НАШЛАСЬ!"));
#endif

            Door->OpenDoor();
        }
    }
}