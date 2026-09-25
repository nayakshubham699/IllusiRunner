// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "IllusiRunnerGameInstance.h"
#include "IllusiRunnerGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	/*if (MoverActor) // MoverActor != nullptr
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
		if (Mover) // Mover != nullptr
		{
			UE_LOG(LogTemp, Display, TEXT("Succesfully found the MoverComponent!!!"));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Failed to find MoverComponent!!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("The MoverActor is null Pointer!!!"));
	}
	*/

	if (IsPressurePlate || IsLava)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/*void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s doesn't have Mover to Trigger!!!"), *GetOwner()->GetActorNameOrLabel());
	}
	
}*/

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsPressurePlate)
	{
		if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
		{
			/*ActivatorCount++;
			if (!IsTriggered)
			{
				Trigger(true);
			}*/
			UE_LOG(LogTemp, Display, TEXT("Actor Has Tag"));
			
			UGameInstance* GameInstance = GetWorld()->GetGameInstance();
			UIllusiRunnerGameInstance* IllusiRunnerGameInstance = Cast<UIllusiRunnerGameInstance>(GameInstance);

			if (IllusiRunnerGameInstance)
			{
				IllusiRunnerGameInstance->LoadNextlevel();

			}
		}
	}
	else if (IsLava)
	{
		UE_LOG(LogTemp, Display, TEXT("Died in Lava"));
		AIllusiRunnerGameMode* IllusiRunnerGameMode = Cast<AIllusiRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (IllusiRunnerGameMode)
		{
			IllusiRunnerGameMode->PlayerDied();
		}
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsPressurePlate)
	{
		if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator"))
		{
			/*ActivatorCount--;
			if (IsTriggered && ActivatorCount == 0)
			{
				Trigger(false);
			}
			*/
		}
	}
	else if (IsLava)
	{

	}
}

