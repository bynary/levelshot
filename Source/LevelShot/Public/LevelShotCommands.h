// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "LevelShotStyle.h"

class FLevelShotCommands : public TCommands<FLevelShotCommands>
{
public:

	FLevelShotCommands()
		: TCommands<FLevelShotCommands>(TEXT("LevelShot"), NSLOCTEXT("Contexts", "LevelShot", "LevelShot Plugin"), NAME_None, FLevelShotStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
