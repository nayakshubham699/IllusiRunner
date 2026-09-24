#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IllusionMovementComponent.generated.h"

class AIllusiRunnerPlayerCharacter;
class AIllusiRunnerMimicCharacter;
class UCharacterMovementComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ILLUSIRUNNER_API UIllusionMovementComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:

    UIllusionMovementComponent();

    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction
    ) override;

    void Move(FVector2D MovementInput);

    void JumpPressed();

    void JumpReleased();

protected:

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Illusion"
    )
    TSubclassOf<AIllusiRunnerMimicCharacter> MimicClass;

private:

    AIllusiRunnerPlayerCharacter* Player;
    AIllusiRunnerMimicCharacter* Mimic;

    UCharacterMovementComponent* PlayerMovement;
    UCharacterMovementComponent* MimicMovement;

    FVector2D CurrentMovementInput;

    bool bMovementInputActive;
    bool bPlayerJumping;

    void UpdateMimicHorizontalMovement();
    void UpdateVerticalMovement();
};