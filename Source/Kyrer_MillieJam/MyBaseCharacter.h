// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpectralInterface.h"
#include "GameHUDWidget.h"
#include "MyBaseCharacter.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;

	FORCEINLINE bool operator==(const FInventoryItem& Other) const
	{
		return ItemID == Other.ItemID;
	}
};

UCLASS()
class KYRER_MILLIEJAM_API AMyBaseCharacter : public ACharacter, public ISpectralInterface
{
	GENERATED_BODY()

protected:
	// Текущая стабильность в призрачном мире (от 0 до 100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Survival")
	float Stability = 100.0f;

	// Время до смены дня/ночи
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Survival")
	float TimeToTimeChange = 60.0f;

	// Количество корма для Милли
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Inventory")
	int32 FoodCount = 0;

	// Наш массив предметов для загадок, использующий структуру инвенторя
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|Inventory")
	TArray<FInventoryItem> KeyItemsInventory;

	// Находимся ли мы сейчас в призрачном мире
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MillieJam|World")
	bool bIsPhantomWorld = false;

	// Контекст ввода (IMC)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputMappingContext* DefaultMappingContext;

	// Действие для движения (IA) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* MoveAction;

	// Действие для взгляда мышкой (IA)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* LookAction;

	// Действие для прыжка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* ToggleWorldAction;

	// да это ПАУЗА!
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* PauseAction;

	// Вызываем чтобы ЗАМЕРЕТЬ! ХАХАХАХХАХ
	UFUNCTION(BlueprintImplementableEvent)
	void Pause();

	// Вызываем для движения
	void Move(const struct FInputActionValue& Value);

	// Вызываем для осмотра
	void Look(const struct FInputActionValue& Value);

	// Действие для взаимодействия (Клавиша E) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|Input")
	class UInputAction* InteractAction;

	// Компоненты для камеры (делаем видимыми для блупринта)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FirstPersonCamera;

	// Вызываем клавишой взаимодействия
	void Interact();

	void ToggleWorld();

	void TogglePauseMenu();
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* MySuperSound;

	UFUNCTION(BlueprintImplementableEvent, Category = "MyEvents")
	void AddFood();

	UFUNCTION(BlueprintCallable)
	void SetPausedState(bool bNewPaused) { bIsPaused = bNewPaused; }

	bool bIsDead = false;
	bool bIsWon = false;
	bool bIsPaused = false;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UGameHUDWidget* MyHUDRef;

	// Sets default values for this character's properties
	AMyBaseCharacter();
	int32 GetFoodCount() const { return FoodCount; }
	void SetFood(int32 Amount);

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void ToggleHUDImage(bool bIsVisible);

	// метод для добавления еды
	void AddFood(int32 Amount);

	// Метод для добавления квест предмета
	void AddKeyItem(FInventoryItem NewItem);

	// Метод для удаления квест предмета
	void RemKeyItem(FInventoryItem NewItem);

	void RestoreStability() 
	{ 
		Stability = 100.0f;
		// Обновляем бар Стабильности на HUD
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			AMyBaseCharacter* Player = Cast<AMyBaseCharacter>(PC->GetPawn());
			if (Player && Player->MyHUDRef)
			{
				Player->MyHUDRef->UpdateStability(Stability / 100.0f); // так и называется
			}
		}
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|World Mechanics")
	class UMaterialParameterCollection* WorldStateMPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MillieJam|World Mechanics")
	float ParameterValue = 0.0f;

	UFUNCTION(BlueprintCallable)
	bool HasItem(FName ItemID) const;

protected:

	UPROPERTY()
	TArray<AActor*> SpectralActors;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
