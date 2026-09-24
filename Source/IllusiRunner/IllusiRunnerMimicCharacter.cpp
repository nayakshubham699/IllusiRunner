#include "IllusiRunnerMimicCharacter.h"

AIllusiRunnerMimicCharacter::AIllusiRunnerMimicCharacter()
{
}

void AIllusiRunnerMimicCharacter::ReceiveMirrorLook(
    float X_Rotation,
    float Y_Rotation)
{
    DoLook(
        -X_Rotation,
        Y_Rotation
    );
}