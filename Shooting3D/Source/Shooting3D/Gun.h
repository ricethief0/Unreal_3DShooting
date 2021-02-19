// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SHOOTING3D_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	inline int32 GetAmmo() const { return Ammo; }
	inline int32 GetAmmoMax() const { return CurruntAmmoMax; }
	void AmmoReload();
	void ChargeAmmoMax(const int32 AmmoCount) { CurruntAmmoMax += AmmoCount; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;


	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactHit;

	UPROPERTY(EditAnywhere)
	float MaxRange = 2000.0f;
	
	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	USoundBase* EmptyAmmoSound;
	UPROPERTY(EditAnywhere)
	int32 AmmoChargeCount = 30;
	UPROPERTY(VisibleAnywhere)
	int32 Ammo;
	UPROPERTY(VisibleAnywhere)
	int32 CurruntAmmoMax = 30;

	
};
