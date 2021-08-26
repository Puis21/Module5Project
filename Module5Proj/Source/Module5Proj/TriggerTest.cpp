// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerTest.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATriggerTest::ATriggerTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Collision"));
	CombatCollision->InitBoxExtent(FVector(40.f, 40.f, 40.f));
	CombatCollision->SetCollisionProfileName("Trigger");
	RootComponent = CombatCollision;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Collision"));
	MyMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATriggerTest::BeginPlay()
{
	Super::BeginPlay();


	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ATriggerTest::OnOverLapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &ATriggerTest::OnOverlapEnd);
	
}

// Called every frame
void ATriggerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerTest::OnOverLapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
		//UE_LOG(LogTemp, Warning, TEXT("ORICE"));
	}

}

void ATriggerTest::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}