// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyInteractableInterface.h"
#include "MillieCat.generated.h"

// Перечисление состояний кошки (Обычная / Спящая)
UENUM(BlueprintType)
enum class EMillieState : uint8
{
	Normal   UMETA(DisplayName = "Normal"),
	Sleep    UMETA(DisplayName = "Sleep")
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCatStarvedDelegate);

UCLASS()
class KYRER_MILLIEJAM_API AMillieCat : public ACharacter, public IMyInteractableInterface
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintAssignable, Category = "MillieJam|Cat")
	FOnCatStarvedDelegate OnCatStarvedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* MySuperSound;

protected:
	virtual void BeginPlay() override; 	// Called when the game starts or when spawned

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Cat")
	float Satiety = 100.0f; 	// Текущая сытость кошки ( от 0 до 100)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Cat")
	float MaxSatiety = 100.0f; 	// Максимальная сытость

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MillieJam|Cat")
	EMillieState CurrentState = EMillieState::Normal; 	// Текущее состояние кошки

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Cat")
	float StarvationRate = 1.0f; 	// Скорость пассивного голодания в секунду (для обычной формы)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|AI")
	TArray<AActor*> CurrentSectionPoints; 	// Массив точек для ТЕКУЩЕЙ секции (выставляем ручками в блупринте или закидываем кодом)

	FTimerHandle MoveTimerHandle;

	void MoveToRandomPoint(); // Функция самого прыжка\перемещения на новую точку
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|AI")
	bool bIsSleeping;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|AI")
	float MoveInterval; // раз в скок секунд кот меняет позицию


	void ChangeRoomPositions(TArray<AActor*> NewPositions);

	UPROPERTY()
	bool bHasStarved = false;

public:	

	UFUNCTION(BlueprintCallable, Category = "MillieJam|AI")
	void UpdateMovementPoints(const TArray<AActor*>& NewPoints, bool bForceTeleportToNewSection);

	UFUNCTION(BlueprintCallable, Category = "MillieJam|AI")
	void OnCatStarved();

	AMillieCat();

	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MillieJam|Cat")
	void Eat(float Amount); 	// Метод восполнения сытости(вызывается игроком)

	UFUNCTION(BlueprintCallable, Category = "MillieJam|Cat")
	void SetCatState(EMillieState NewState); 	// Изменение состояния кошки (Будет вызываться при смене мира)

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	class AAIController* MyAIController; 	// Указатель на контроллер ИИ, который будет рулить ногами кота

};
