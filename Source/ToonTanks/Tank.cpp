// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());
}

APlayerController *ATank::GetTankPlayerController() const
{
    return TankPlayerController;
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TankPlayerController)
    {
        FHitResult HitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

        // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25, 12, FColor::Red, false, -1);

        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}

void ATank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;

    DeltaLocation.X = Value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;

    DeltaRotation.Yaw = Value * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);

    AddActorLocalRotation(DeltaRotation, true);
}
