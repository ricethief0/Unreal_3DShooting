// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Engine/World.h"
#include "Gun.h"
#include "Ammo.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBarWidget"));
	HpBarWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket")); // 메쉬에 총을 붙여준다.
	//메쉬에 붙여줄 총에 이름을 3번째 인자에 쓰고, 지역성트랜스폼 형태로 두기 위해 2번째 인자를 저렇게 사용한다.
	Gun->SetOwner(this);///

	
	Health = HealthMax;
	
	if (GetController() != nullptr &&  Cast<AShooterAIController>(GetController()) != nullptr)
	{
		ProgressBar = Cast<UProgressBar>(HpBarWidget->GetUserWidgetObject()->GetWidgetFromName(TEXT("Enemy_HP_ProgressBar")));
		HpBarWidget->SetVisibility(false);
		
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealthRegain(DeltaTime);
	
	if (HpBarWidget == nullptr) return;
		HpBarView(DeltaTime);
	
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::Reload);
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0 ;
}


void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector()*AxisValue);
	
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Reload()
{
	Gun->AmmoReload();
}

void AShooterCharacter::HealthRegain(float DeltaTime)
{
	if (GetController() != nullptr && GetController()->GetPawn()->IsPlayerControlled() && Health <= HealthMax && !IsDie)
	{
		HealthCounter += DeltaTime;

		if (HealthCounter >= HealthTime)
		{
			Health++;
		}
	}
}

void AShooterCharacter::AmmoAppear()
{
	AmmoActor = GetWorld()->SpawnActor<AAmmo>(AmmoClass);
	AmmoActor->SetActorLocation(GetActorLocation());
	//AmmoActor->SetOwner(this);///
}

float AShooterCharacter::GetHPPercent() const
{
	return Health / HealthMax;
}

FString AShooterCharacter::GetAmmo() const
{
	return FString::Printf(TEXT("Ammo  %d/%d"), Gun->GetAmmo(), Gun->GetAmmoMax());
}

void AShooterCharacter::Shoot()
{
	Gun->PullTrigger();
}

void AShooterCharacter::AmmoCharge(const int32 AmmoCount)
{
	Gun->ChargeAmmoMax(AmmoCount);
}



float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = 	Super::TakeDamage( DamageAmount,   DamageEvent,  EventInstigator,  DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("HP : %f.0f"), Health);

	HealthCounter = 0.f;

	
	if (GetController() != nullptr && Cast<AShooterAIController>(GetController()) != nullptr)
	{
		ProgressBar->SetPercent(GetHPPercent());
		HpBarWidget->SetVisibility(true);
		HitCount = 0.f;
	}

	if (IsDead())
	{
		IsDie = true;

		if (GetController() != nullptr && Cast<AShooterAIController>(GetController()) != nullptr)
		{
			AmmoAppear();
			HpBarWidget->SetVisibility(false);

		}
		
		
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}


	return DamageToApply;
}

void AShooterCharacter::HpBarView(float DeltaTime)
{
	
	if (!HpBarWidget->IsVisible())return;
	
	HitCount += DeltaTime;
	if (HitCount >= HpBarViewDelay)
	{
		HitCount = 0.f;
		HpBarWidget->SetVisibility(false);
	}
	
}



