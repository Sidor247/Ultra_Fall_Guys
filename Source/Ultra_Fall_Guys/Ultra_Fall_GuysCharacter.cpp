// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ultra_Fall_GuysCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUltra_Fall_GuysCharacter

AUltra_Fall_GuysCharacter::AUltra_Fall_GuysCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUltra_Fall_GuysCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HUD_Class)
		if (auto hud = CreateWidget<UHUD_Base>(GetWorld(), HUD_Class))
		{
			HUD = hud;
			HUD->AddToViewport();
			HUD->Set_Health(Health);
		}
}

void AUltra_Fall_GuysCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	Handle_Death();
	Super::FellOutOfWorld(dmgType);
}

void AUltra_Fall_GuysCharacter::Handle_Death()
{
	GetMesh()->SetSimulatePhysics(true);
	
	if (HUD)
		HUD->RemoveFromViewport();
	if (Total_Screen_Class)
		if (auto total_screen = CreateWidget<UTotal_Screen_Base>(GetWorld(), Total_Screen_Class))
		{
			Total_Screen = total_screen;
			Total_Screen->AddToViewport();
			Total_Screen->Set_Result(false);
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(controller, Total_Screen, EMouseLockMode::LockOnCapture, true);
			controller->SetShowMouseCursor(true);
		}
}

void AUltra_Fall_GuysCharacter::Handle_Finish()
{
	if (HUD)
		HUD->RemoveFromViewport();
	if (Total_Screen_Class)
		if (auto total_screen = CreateWidget<UTotal_Screen_Base>(GetWorld(), Total_Screen_Class))
		{
			Total_Screen = total_screen;
			Total_Screen->AddToViewport();
			Total_Screen->Set_Result(true, Play_Time_Seconds);
			APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(controller, Total_Screen, EMouseLockMode::LockOnCapture, true);
			controller->SetShowMouseCursor(true);
		}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUltra_Fall_GuysCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUltra_Fall_GuysCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUltra_Fall_GuysCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUltra_Fall_GuysCharacter::Handle_Damage(int Damage)
{
	Health -= Damage;
	if (Health < 0)
		Health = 0;
	if (HUD)
		HUD->Set_Health(Health);
	if (Health == 0)
		Handle_Death();
}

void AUltra_Fall_GuysCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUltra_Fall_GuysCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUltra_Fall_GuysCharacter::Tick(float DeltaSeconds)
{
	Play_Time_Seconds += DeltaSeconds;
	if (HUD)
		HUD->Set_Time(Play_Time_Seconds);
}
