// Fill out your copyright notice in the Description page of Project Settings.


#include "MyKeyDoor.h"
#include "MyBaseCharacter.h"

// Sets default values
AMyKeyDoor::AMyKeyDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// создаем и крепим меш
	DoorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DoorRoot;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(DoorRoot);

	// Настраиваем коллизию, чтобы LineTrace (ECC_Visibility) его видел
	MeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AMyKeyDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyKeyDoor::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{
	AMyBaseCharacter* Player = Cast<AMyBaseCharacter>(Interactor);


	if (Player != nullptr && Player->HasItem(RequiredKeyID))
	{
		OpenDoor();
	}
	else
	{
		if (GEngine)
		{

			NoKey();
#if !UE_BUILD_SHIPPING
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("У вас нет нужного ключа!"));
#endif
		}
	}
}

// Called every frame
void AMyKeyDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

