// Fill out your copyright notice in the Description page of Project Settings.

#include "MillieCat.h"
#include "MyBaseCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AMillieCat::AMillieCat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMillieCat::BeginPlay()
{
	Super::BeginPlay();
	Satiety = MaxSatiety;

	bIsSleeping = false;
	MoveInterval = 30.0f;

	// Заставляем движок спавнить ии контролер для этого кота
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Запускаем цикличный таймер ан перемещение кота
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AMillieCat::MoveToRandomPoint, MoveInterval, true);

	// Кешируем контролер кота
	MyAIController = Cast<AAIController>(GetController());

	// Запускаем цикличный таймер ИИ
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AMillieCat::MoveToRandomPoint, MoveInterval, true);
}

// Универсальный метод перезаписи. Вызывать его может хоть GameMode, хоть триггер двери
void AMillieCat::UpdateMovementPoints(const TArray<AActor*>& NewPoints, bool bForceTeleportNewSection)
{
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Чистим пул..."));
#endif

	// 1. Начисто чистим старый пул
	CurrentSectionPoints.Empty();
	
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Записывай новый пул..."));
#endif

	// 2. Перезаписываем новый пришедшим массивам
	CurrentSectionPoints = NewPoints;

#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Проверка на баг..."));
#endif

	// 3. Защита от бага со спячкой: если принудительно переходим в новую секцию
	if (bForceTeleportNewSection && CurrentSectionPoints.Num() > 0)
	{

#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Останавливаем кота..."));
#endif

		// Если кот ушёл куда-то в старой секции - принудительно останавливаем навигацию
		if (MyAIController)
		{
			MyAIController->StopMovement();
		}

#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Будим кота..."));
#endif

		//Кот гарантированно просыпается чотбы не ломать логику
		bIsSleeping = false;

#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Выбираем точку..."));
#endif

		// ВЫбираем новую точку из свежего массива
		int32 RandomIndex = FMath::RandRange(0, CurrentSectionPoints.Num() - 1);
		AActor* SpawnPoint = CurrentSectionPoints[RandomIndex];

		if (SpawnPoint)
		{
			// Телепортируем кота в новую секцию, сбрасываего физику движения
			SetActorLocation(SpawnPoint->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);

#if !UE_BUILD_SHIPPING
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Кот успешно перенесён в новую секцию!"));
#endif
		}

	}
}

void AMillieCat::MoveToRandomPoint()
{
	// Если кот спит - он никуда не идёт (работает во всех обычных ситуациях)
	if (bIsSleeping)
	{

#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Кот никуда не пойдёт! Он спит!"));
#endif

		return;
	}

	// Проверяем, есть ли точки в массиве(нужно заполнять в редакторе) и готов ли ии контроллер
	if (CurrentSectionPoints.Num() == 0 || !MyAIController) 
	{

#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Массив позиций кота пустой!"));
#endif

		return;
	}

	// Выбираем случайный индекс из массива
	int32 RandomIndex = FMath::RandRange(0, CurrentSectionPoints.Num() - 1);
	AActor* TargetPoint = CurrentSectionPoints[RandomIndex];

	if (TargetPoint)
	{
		// Магия навигации: кот физически разворачивается и идёт к точке обходя препятствия
		MyAIController->MoveToActor(TargetPoint, 5.0f); // 5.0f - радиус остановки в сантиметрах
#if !UE_BUILD_SHIPPING
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Кот идёт на новую позицию!"));
#endif
	}
}

void AMillieCat::OnCatStarved() // BlueprintCallable — вызывается ИЗ C++ Tick
{
	OnCatStarvedDelegate.Broadcast(); // а дальше делегат уведомляет BP-слушателей
}

// Called every frame
void AMillieCat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Расчитываем множитель голода в зависимости от формы
	float CurrentStarvationRate = StarvationRate;
	if (CurrentState == EMillieState::Sleep)
	{
		CurrentStarvationRate /= 2.0f; // В спящем режиме кошка медленее голодает
	}
	
	// Уменьшаем сытость
	Satiety = FMath::Clamp(Satiety - (CurrentStarvationRate * DeltaTime), 0.0f, MaxSatiety);

	// Обновляем бар сытости на HUD
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		AMyBaseCharacter* Player = Cast<AMyBaseCharacter>(PC->GetPawn());
		if (Player && Player->MyHUDRef)
		{
			Player->MyHUDRef->UpdateSatiety(Satiety / MaxSatiety); // А Функция называется прям так)))
		}
	}

	if (Satiety <= 0.0f && !bHasStarved)
	{
		bHasStarved = true;
		OnCatStarved();
	}

#if !UE_BUILD_SHIPPING
	if (GEngine)
	{
		FString StateStr = (CurrentState == EMillieState::Normal) ? TEXT("Бодрствует") : TEXT("Спит");
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Orange, FString::Printf(TEXT("Милли (%s). Сытость: %.1f %%"), *StateStr, Satiety));
	}
#endif
}

void AMillieCat::Eat(float Amount)
{
	Satiety = FMath::Clamp(Satiety + Amount, 0.0f, MaxSatiety);
}

void AMillieCat::SetCatState(EMillieState NewState)
{
	CurrentState = NewState;
}

// Called to bind functionality to input
void AMillieCat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMillieCat::OnInteract_Implementation(AMyBaseCharacter* Interactor)
{
	if (!IsValid(Interactor)) return;

	Interactor->RestoreStability();
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Разум чист и полон сил!"));
#endif



	// Проверяем есть ли у персонажа еда в инвентаре(FoodCount)
	int32 CatFood = Interactor->GetFoodCount();
	if (CatFood > 0)
		{
			int32 LastFood = Satiety;

			Eat(CatFood);

			int32 DiffFood = Satiety - LastFood;


			Interactor->SetFood(CatFood - DiffFood);
			if (MySuperSound)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), MySuperSound);
			}
	#if !UE_BUILD_SHIPPING
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Милли успешно покормлена!"));
	#endif
		}
		else
		{
	#if !UE_BUILD_SHIPPING
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Нет корма для Милли!"));
	#endif
		}
}

