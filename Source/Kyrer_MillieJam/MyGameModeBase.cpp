// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MillieCat.h"
#include "Kismet/GameplayStatics.h"

AMyGameModeBase::AMyGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}


void AMyGameModeBase::SwitchTimeOfDay()
{
	AActor* FoundCat = UGameplayStatics::GetActorOfClass(GetWorld(), AMillieCat::StaticClass());
	if (AMillieCat* Millie = Cast<AMillieCat>(FoundCat))
	{
		if (CurrentTimeState == ETimeOfDay::Day)
		{
			if (IsValid(Millie)) { Millie->SetCatState(EMillieState::Sleep); } // Днем бодрствует
			CurrentTimeState = ETimeOfDay::Night;
			TimeRemaining = 18.0f; // НАступила ночь на 3 минуты (180 сек)

		}
		else
		{
			if (IsValid(Millie)) { Millie->SetCatState(EMillieState::Normal); } // Ночью уходит в режим сна
			CurrentTimeState = ETimeOfDay::Day;
			TimeRemaining = 30.0f; // НАступил день на 5 минуты (300 сек)
		}
#if !UE_BUILD_SHIPPING
		if (GEngine)
		{
			FString TimeStateStr = (CurrentTimeState == ETimeOfDay::Day) ? TEXT("День") : TEXT("Ночь");
			GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Orange, FString::Printf(TEXT("Наступила (%s). Таймер: %.1f %%"), *TimeStateStr, TimeRemaining));
		}
#endif
	}
	OnTimeOfDayChanged(CurrentTimeState);
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TimeRemaining > 0.0f)
	{
		TimeRemaining -= DeltaTime;

		if (TimeRemaining <= 0.0f)
		{
			// Таймер кончился! переключаем время суток
			SwitchTimeOfDay();
		}
	}
}
