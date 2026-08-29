// Fill out your copyright notice in the Description page of Project Settings.


#include "MySectionKey.h"
#include "MyWorldSubsystem.h"

// Sets default values
AMySectionKey::AMySectionKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMySectionKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMySectionKey::OnInteract_Implementation(class AMyBaseCharacter* Interactor)
{
    if (UWorld* World = GetWorld())
    {
        if (UMyWorldSubsystem* Subsystem =
            World->GetSubsystem<UMyWorldSubsystem>())
        {
            Subsystem->GiveKey(KeyID);
        }
    }

    OnKeyCollected();
	// Самоуничтожение предмета
	Destroy();
}

void AMySectionKey::OnKeyCollected_Implementation()
{
    // логика: дать ключ игроку, спрятать/уничтожить актора, звук и т.д.
}

// Called every frame
void AMySectionKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

