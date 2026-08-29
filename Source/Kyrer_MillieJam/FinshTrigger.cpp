// Fill out your copyright notice in the Description page of Project Settings.


#include "FinshTrigger.h"
#include "Components/BoxComponent.h"
#include "MyBaseCharacter.h"

// Sets default values
AFinshTrigger::AFinshTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StepVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("StepVolume"));
	RootComponent = StepVolume;
}

// Called when the game starts or when spawned
void AFinshTrigger::BeginPlay()
{
	Super::BeginPlay();
	if (StepVolume)
	{
		StepVolume->OnComponentBeginOverlap.AddDynamic(this, &AFinshTrigger::OnOverlapBegin);
		StepVolume->OnComponentEndOverlap.AddDynamic(this, &AFinshTrigger::OnOverlapEnd);
	}
}

void AFinshTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ЕСТЬ ВХОД!"));
#endif
	// Проверяем, что в триггер зашел именно игрок, а не коробка или враг
	if (OtherActor && OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ПРОШЛО! СИГНАЛ ПУЩЕН!"));
#endif
		// БРОСАЕМ СИГНАЛ В ЭФИР! Нода Broadcast — это и есть вызов делегата
		OnPlayerReachedFinish.Broadcast();
	}
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("НЕПРОШЛО!"));
#endif
}

void AFinshTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("ЕСТЬ ВЫХОД!"));
#endif
	AMyBaseCharacter* Player = Cast<AMyBaseCharacter>(OtherActor);
	if (Player)
	{
		Player->ToggleHUDImage(false);
	}
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("НЕПРОШЛО!"));
#endif
}


// Called every frame
void AFinshTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

