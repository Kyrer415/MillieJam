// Fill out your copyright notice in the Description page of Project Settings.


#include "GhosButton.h"
#include "MyWorldSubsystem.h"

// Sets default values
AGhosButton::AGhosButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// создаем и крепим меш
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Настраиваем коллизию, чтобы LineTrace (ECC_Visibility) его видел
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AGhosButton::BeginPlay()
{
	Super::BeginPlay();

}

void AGhosButton::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{
    if (UWorld* World = GetWorld())
    {
#if !UE_BUILD_SHIPPING
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("ВХОД!")));
#endif

        if (UMyWorldSubsystem* Subsystem =
            World->GetSubsystem<UMyWorldSubsystem>())
        {
            Subsystem->GiveKey(KeyID);
        }
    }
    OnInteracted();
}

// Called every frame
void AGhosButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}