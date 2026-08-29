// Fill out your copyright notice in the Description page of Project Settings.


#include "MySectionalDoor.h"
#include "MyWorldSubsystem.h"

// Sets default values
AMySectionalDoor::AMySectionalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySectionalDoor::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        if (UMyWorldSubsystem* Subsystem = World->GetSubsystem<UMyWorldSubsystem>())
        {
            Subsystem->RegisterSectionDoor(
                DoorID,
                this
            );
        }
    }
}

// Called every frame
void AMySectionalDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}