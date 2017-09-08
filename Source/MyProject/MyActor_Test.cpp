// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor_Test.h"



// Sets default values
AMyActor_Test::AMyActor_Test()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	this->PrimaryActorTick.bCanEverTick = true;

	speed = 1;

	this->SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene_Component"));
	this->RootComponent = SceneComponent;
	this->SampleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SampleMesh_Test")); //Iteam Mesh Creation
	this->SampleMesh->AttachTo(this->RootComponent);
	this->Rotation_Rate = FRotator(0.0f, 180.0f, 0.0f);

	//Creat Collider
	this->BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	this->BoxCollider->bGenerateOverlapEvents = true;
	this->BoxCollider->SetWorldScale3D(FVector(1.0f ,1.0f ,1.0f));
	this->BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AMyActor_Test::OnOverlapBegin);
	this->BoxCollider->AttachToComponent(this->RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);


}

void AMyActor_Test::OnInteract()
{
	FString PickUpName = FString::Printf(TEXT("Picked up: %s"), *Name);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PickUpName);
	Destroy();
}

//Overlap Handler
void AMyActor_Test::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Checkif the otherActor is not me and if it is not Null
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

		OnInteract();


		/*
		//Creating a String which saving inside the "Picked up: %s" and gets the name of the item
		FString PickUpName = FString::Printf(TEXT("Picked up: %s"), *GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, PickUpName); //prints out the PickupName string 
		Destroy(); //Destroys the item MyActor_Test
		*/
	}



}

// Called when the game starts or when spawned
void AMyActor_Test::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->AddActorLocalRotation(this->Rotation_Rate * DeltaTime * speed);

	GEngine->AddOnScreenDebugMessage(-10, 5.f, FColor::Red, TEXT("Tick"));

}


