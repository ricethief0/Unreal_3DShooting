// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING3D_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	bool IsDead();
	
protected:
	virtual void  BeginPlay() override;
	
private:
	/*UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 200.f;*/

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	//UPROPERTY(EditAnywhere)
	//	TSubclassOf<class UUserWidget> EnemyHUDClass;

	//UPROPERTY()
	//	UUserWidget* EnemyHUD;

	UPROPERTY()
		bool bIsDamage = false;


};
