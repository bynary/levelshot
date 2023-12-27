// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelShotStyle.h"
#include "LevelShot.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FLevelShotStyle::StyleInstance = nullptr;

void FLevelShotStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FLevelShotStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FLevelShotStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("LevelShotStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FLevelShotStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("LevelShotStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("LevelShot")->GetBaseDir() / TEXT("Resources"));

	Style->Set("LevelShot.PluginAction", new IMAGE_BRUSH_SVG(TEXT("LevelShotIcon"), Icon20x20));
	return Style;
}

void FLevelShotStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FLevelShotStyle::Get()
{
	return *StyleInstance;
}
