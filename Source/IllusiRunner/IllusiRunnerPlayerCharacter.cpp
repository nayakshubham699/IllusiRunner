#include "IllusiRunnerPlayerCharacter.h"

#include "IllusionMovementComponent.h"

AIllusiRunnerPlayerCharacter::AIllusiRunnerPlayerCharacter()
{
    IllusionMovementComponent =
        CreateDefaultSubobject<UIllusionMovementComponent>(
            TEXT("IllusionMovementComponent")
        );
}

void AIllusiRunnerPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AIllusiRunnerPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}