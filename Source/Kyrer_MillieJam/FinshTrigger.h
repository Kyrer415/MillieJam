// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" 
#include "FinshTrigger.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFinishSignature);
UCLASS()
class KYRER_MILLIEJAM_API AFinshTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFinshTrigger();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FFinishSignature OnPlayerReachedFinish;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Components")
	class UBoxComponent* StepVolume;

	// Функция, которая вызовется при входе в триггер
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Функция, которая вызовется при выходе из триггера
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
