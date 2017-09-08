// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProjectCharacter.h"
#include "Engine.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


//////////////////////////////////////////////////////////////////////////
// AMyProjectCharacter

AMyProjectCharacter::AMyProjectCharacter()
{

	ZoomLength = 300.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = ZoomLength; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}


 
//////////////////////////////////////////////////////////////////////////
// Input

void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//------------------------------------CAMERA-------------------------
	//added by me 
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &AMyProjectCharacter::ZoomIn);//added by me 
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &AMyProjectCharacter::ZoomOut);//added by me 

	//------------------------INVENTORY------------------------------------------

	PlayerInputComponent->BindAction("Pick_up", IE_Pressed, this, &AMyProjectCharacter::BeginPick_up);
	PlayerInputComponent->BindAction("EndPick_up", IE_Pressed, this, &AMyProjectCharacter::EndPick_up);
	PlayerInputComponent->BindAction("Show_Inventory", IE_Pressed, this, &AMyProjectCharacter::Show_Inventory);



	//----------------------------------------------------------------------------
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProjectCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProjectCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyProjectCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMyProjectCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProjectCharacter::OnResetVR);
}


void AMyProjectCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMyProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMyProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
//Added by me for camera
void AMyProjectCharacter::ZoomIn(){
	if (CameraBoom->TargetArmLength > 100.0f)
	{
		ZoomLength -= 150.0f;
		if (ZoomLength < 350.0f)
			ZoomLength = 350.0f;
	
	}
}
//Added by me for camera
void AMyProjectCharacter::ZoomOut(){

	if (CameraBoom->TargetArmLength < 1000.0f)
	{
		ZoomLength += 150.0f;

		if (ZoomLength > 1000.0f)
			ZoomLength = 1000.0f;
	}
	
		
}

void AMyProjectCharacter::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
	this->CameraBoom->TargetArmLength = FMath::FInterpTo(this->CameraBoom->TargetArmLength, ZoomLength, Deltatime, 9.0f);
}

//---------------------------------INVENTORY----------------------------
void AMyProjectCharacter::BeginPick_up() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Start_pick_up!!!!!!!!"));

	b_IsPickingUp = true;
}

void AMyProjectCharacter::EndPick_up() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("End_pick_up!"));

	b_IsPickingUp =false;


}
void AMyProjectCharacter::Show_Inventory() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is inventory"));

}