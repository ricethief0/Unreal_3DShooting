// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class UProgressBar;
class UWidgetComponent;
UCLASS()
class SHOOTING3D_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
		bool IsDead()const;
	
	UFUNCTION(BlueprintPure)
		float GetHPPercent()const;
	UFUNCTION(BlueprintPure)
		FString GetAmmo() const;

	void Shoot();

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);
	void Reload();
	void HealthRegain(float DeltaTime);
	
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void HpBarView(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	
	UPROPERTY(EditDefaultsOnly)
		float HealthMax;

	UPROPERTY(VisibleAnywhere)
		float Health;

	UPROPERTY(EditAnywhere)
		float HealthTime = 5.0f;
	UPROPERTY(VisibleAnywhere)
		float HealthCounter = 0.f;

	UPROPERTY()
	AGun* Gun;
	UPROPERTY()
	bool IsDie = false;

	UPROPERTY()
	UProgressBar* ProgressBar;
	
	UPROPERTY(EditAnywhere)
	UWidgetComponent* HpBarWidget;

	UPROPERTY()
	float HitCount = 0.f;
	UPROPERTY(EditAnywhere)
	float HpBarViewDelay = 5.f;

};
