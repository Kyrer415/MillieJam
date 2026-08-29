// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h" 
#include "Camera/CameraComponent.h"
#include "MillieCat.h"
#include "CombinationItem.h"
#include "Sound/SoundBase.h" // Обязательно для указателей на звук
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h" // Нужен для быстрого поиска кошки на уровне
#include "MyInteractableInterface.h"



// Sets default values
AMyBaseCharacter::AMyBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаём камеру от первого лица
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	// Крепим её явно к капсуле персонажа (а не просто к руткомпонент)
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	// Поднимаем камеру на уровень глаз - используем встроенное свойство ACharacter, которое само подтсроится если позже изменить размер капуслы
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));

	// Заставляем Камеру вращатся вслед за движениями мышки и контроллера!
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Родное свойство класса Character: вращать ли само ТЕЛО влево\вправо за мышкой?
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false; // Чтобы тело не наколнялось вверх\вниз вслед за взглядом
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// находим все акторы на уровне
	TArray <AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	// фильтруем только те, у которых есть наш интерфейс, и сохраняем в масив SpectralVision
	for (AActor* Actor : AllActors)
	{
		if (Actor && Actor->GetClass()->ImplementsInterface(USpectralInterface::StaticClass()))
		{
			SpectralActors.Add(Actor);
		}
	}

	// Сразу принудительно прям все нужные объекты
	for (AActor* Actor : SpectralActors)
	{
		if (Actor)
		{
			// передаём false, чтобы призрачные объекты спрятались при старте
			ISpectralInterface::Execute_ToggleSpectralMode(Actor, false);
		}
	}

	// фильтруем только те, у которых есть наш интерфейс, и сохраняем в масив SpectralVision
	for (AActor* Actor : AllActors)
	{
		if (Actor && Actor->GetClass()->ImplementsInterface(USpectralInterface::StaticClass()))
		{
			SpectralActors.Add(Actor);
		}
	}

	if (IsValid(GetCapsuleComponent()))
	{
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	}
	// Проверяем контроллер (переменная с маленькой буквы playerController)
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Приводим стандартный компонент ввода к Enhanced Input
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Взаимодействие
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMyBaseCharacter::Interact);

		// Прыжок
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Движение и Взгляд
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyBaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyBaseCharacter::Look);
		// Переключение мира
		EnhancedInputComponent->BindAction(ToggleWorldAction, ETriggerEvent::Started, this, &AMyBaseCharacter::ToggleWorld);
		// Это пауза МэнЧИК
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AMyBaseCharacter::TogglePauseMenu);

	}
}

void AMyBaseCharacter::Move(const FInputActionValue& Value)
{
	// Получаем векотр движения (X для вперёд\назад, y для право\лево)
	FVector2d MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Узнаём, куда смотрим контроллер (игнорируя накловн вверх\вниз)
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Получаем направление Вперёд
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Получаем направление Вправо
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Добавляем движение персонажу
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void AMyBaseCharacter::Look(const FInputActionValue& Value)
{
	// Получаем движение мыши
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Поворот камеры право\лево и верз\вниз
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyBaseCharacter::Interact()
{
#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Кнопка E нажата!"));
#endif


	// 1. Проверяем камеру персонажа
	if (!IsValid(FirstPersonCamera)) return;

	// 2. Настраиваем начальную точку луча (глаза) и конечную (вперед на 250 см)
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector ForwardVector = FirstPersonCamera->GetForwardVector();
	FVector End = Start + (ForwardVector * 250.0f);

	// 3. Сюда запишутся результаты попадания луча
	FHitResult HitResult;

	// Настройка луча: игнорируем самого себя(Персонажа), чтобы луч не бился об наше тело
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// 4. Пускаем луч по стандартному каналу видимости (ECC_Visiblitity)
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	// Для теста рисуем линию луча (Дебаг-линия: полностью Исчезнет в релизном билде игры (Shipping))
#if !UE_BUILD_SHIPPING
	FColor LineColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 1.0f);
#endif

	// Проверяем, попали ли мы в какой-то объект
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		FString ActorName = HitActor->GetName();

		if (IsValid(HitActor))
		{

			if (HitActor->GetClass()->ImplementsInterface(UMyInteractableInterface::StaticClass()))
			{
				// Вызываем интерфейс взаимодействия и передаем туда себя(this(self))
				IMyInteractableInterface::Execute_OnInteract(HitActor, this);

				// Выводим имя объекта на экран фиолетовым цветом
#if !UE_BUILD_SHIPPING
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, FString::Printf(TEXT("Посмотрел на: %s"), *ActorName));
				}
#endif
			}
		}
	}
}

void AMyBaseCharacter::SetFood(int32 Amount)
{
	FoodCount = Amount;
}

void AMyBaseCharacter::AddFood(int32 Amount)
{
	FoodCount = FoodCount + Amount;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Food: %d"), FoodCount));
	AddFood();
}

void AMyBaseCharacter::AddKeyItem(FInventoryItem NewItem)
{
	KeyItemsInventory.Add(NewItem);

#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Подобран предмет: %s"), *NewItem.ItemName));
#endif
}

void AMyBaseCharacter::RemKeyItem(FInventoryItem NewItem)
{
	KeyItemsInventory.Remove(NewItem);

#if !UE_BUILD_SHIPPING
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Отдан предмет: %s"), *NewItem.ItemName));
#endif
}

void AMyBaseCharacter::ToggleWorld()
{
	// запрещаем вход если стабильность равна нулю
	if (!bIsPhantomWorld && Stability <= 0.0f)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("STABILITY DEPLETED! PET THE MILLIE!."));
		return;
	}


	bIsPhantomWorld = !bIsPhantomWorld;

	// Твой For Each Loop со скриншота, но на C++ без лагов
	for (AActor* Actor : SpectralActors)
	{
		if (Actor)
		{
			// Дергаем метод интерфейса у каждого куба/загадки
			ISpectralInterface::Execute_ToggleSpectralMode(Actor, bIsPhantomWorld);
		}
	}

	if (bIsPhantomWorld)
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	}

#if !UE_BUILD_SHIPPING
	if (GEngine)
	{
		FString WorldName = bIsPhantomWorld ? TEXT("Призрачный!") : TEXT("Реальный!");
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, bIsPhantomWorld ? FColor::Purple : FColor::Orange, FString::Printf(TEXT("Мир изменён на: %s"), *WorldName));
	}
#endif
}

bool AMyBaseCharacter::HasItem(FName ItemID) const
{
	for (const FInventoryItem& Item : KeyItemsInventory)
	{
		if (Item.ItemID == ItemID)
		{
			return true;
		}
	}

	return false;
}

// Called every frame
void AMyBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Каждую секунду уменьшаем таймер дня\ночи (эту логику мы потом свяжем с Домом)
	if (TimeToTimeChange > 0.0f)
	{
		TimeToTimeChange -= DeltaTime;
	}

	// Логика стабильности в призрачном мире
	if (bIsPhantomWorld)
	{
		// В прищрачном мире стабильность падает
		Stability = FMath::Clamp(Stability - (5.0f * DeltaTime), 0.0f, 100.0f);

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

		if (Stability <= 0)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Magenta, TEXT("STABILITY DEPLETED! PET MILLIE!"));
			ToggleWorld();
		}
		else
		{
			// В реальном мире стабильность не восстанавливается
			Stability = FMath::Clamp(Stability + (0.0f * DeltaTime), 0.0f, 100.0f);
		}
#if !UE_BUILD_SHIPPING
		if (GEngine)
		{
			// выводим стабильность в отдельную строку (ID=2) чтобы не спасить лог каждый кадр
			GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Cyan, FString::Printf(TEXT("Стабильность разума: %.1f %%"), Stability));
		}
#endif
	}
}

void AMyBaseCharacter::TogglePauseMenu()
{
	if (bIsDead || bIsWon) return;
	if (!MyHUDRef) return; 

	bIsPaused = !bIsPaused;
	MyHUDRef->ShowPauseMenu(bIsPaused);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (bIsPaused)
	{
		// Переключаем на Game And UI вместо UI Only!
		FInputModeGameAndUI InputMode;

		// Настраиваем, чтобы кнопки виджета ловили клики мыши сразу, а не со второго щелчка
		InputMode.SetWidgetToFocus(MyHUDRef->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		// Возвращаем чистый игровой ввод при снятии
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);
}