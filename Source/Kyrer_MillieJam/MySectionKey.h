// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInteractableInterface.h"
#include "MySectionKey.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API AMySectionKey : public AActor, public IMyInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySectionKey();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
	int32 KeyID;

	UFUNCTION(BlueprintNativeEvent)
	void OnKeyCollected();
	void OnKeyCollected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Реализация функции интефрейса. _Implementation обязателен дляв BlueprintNativeEvent!
	virtual void OnInteract_Implementation(class AMyBaseCharacter* Interactor) override;
};
