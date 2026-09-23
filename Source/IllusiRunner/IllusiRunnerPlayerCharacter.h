#pragma once

#include "CoreMinimal.h"
#include "IllusiRunnerCharacter.h"
#include "IllusiRunnerPlayerCharacter.generated.h"

class UIllusionMovementComponent;

UCLASS()
class ILLUSIRUNNER_API AIllusiRunnerPlayerCharacter
    : public AIllusiRunnerCharacter
{
    GENERATED_BODY()

public:

    AIllusiRunnerPlayerCharacter();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

protected:

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Illusion"
    )
    UIllusionMovementComponent* IllusionMovementComponent;
};