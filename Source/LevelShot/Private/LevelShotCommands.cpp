// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelShotCommands.h"

#define LOCTEXT_NAMESPACE "FLevelShotModule"

void FLevelShotCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "LevelShot", "Execute LevelShot action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
