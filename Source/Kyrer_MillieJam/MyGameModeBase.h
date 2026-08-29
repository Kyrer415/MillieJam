// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpectralInterface.h"
#include "MyGameModeBase.generated.h"

UENUM(BlueprintType)
enum class ETimeOfDay : uint8
{
	Day,
	Night
};



UCLASS()
class KYRER_MILLIEJAM_API AMyGameModeBase : public AGameModeBase, public ISpectralInterface
{
	GENERATED_BODY()
	
protected:
	// Создаём переменную для таймера
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float TimeRemaining = 30.0f; // 5 минут

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	ETimeOfDay CurrentTimeState = ETimeOfDay::Day;


	// Это событие вызовим в c++, и настроим внутри блупринта GameMode	
	UFUNCTION(BlueprintImplementableEvent, Category = "Time")
	void OnTimeOfDayChanged(ETimeOfDay NewTimeState);

	void SwitchTimeOfDay();


public:
	AMyGameModeBase(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
};
