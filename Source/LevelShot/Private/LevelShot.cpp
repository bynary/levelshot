// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelShot.h"
#include "LevelShotStyle.h"
#include "LevelShotCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

static const FName LevelShotTabName("LevelShot");

#define LOCTEXT_NAMESPACE "FLevelShotModule"

void FLevelShotModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLevelShotStyle::Initialize();
	FLevelShotStyle::ReloadTextures();

	FLevelShotCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLevelShotCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FLevelShotModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLevelShotModule::RegisterMenus));
}

void FLevelShotModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FLevelShotStyle::Shutdown();

	FLevelShotCommands::Unregister();
}

void FLevelShotModule::PluginButtonClicked()
{
    // Code from Jamie Holding - Sept 9, 2019
    // https://blog.jamie.holdings/2019/09/08/taking-screenshots-in-game-in-unreal-engine-4-23/
    const FDateTime Now = FDateTime::Now();
    // store screenshot in Project directory next to main UProject/EXE based on the build type
    #if WITH_EDITOR
    const FString ImageDirectory = FString::Printf(TEXT("%s/%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
    #else
    const FString ImageDirectory = FString::Printf(TEXT("%s/../%s"), *FPaths::ProjectDir(), TEXT("Screenshots"));
    #endif

    const FString ImageFilename = FString::Printf(TEXT("%s/Screenshot_%d%02d%02d_%02d%02d%02d_%03d.png"), *ImageDirectory, Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond(), Now.GetMillisecond());

    FScreenshotRequest::RequestScreenshot(ImageFilename, false, false);

	// Notify the user that the screenshot was taken.
	FNotificationInfo Info(LOCTEXT("ScreenshotFinished", "A screenshot has been created."));
	Info.FadeInDuration = 0.5f;
	Info.FadeOutDuration = 0.5f;
	Info.ExpireDuration = 5.0f;
	Info.bUseThrobber = false;
	Info.bUseSuccessFailIcons = true;
	Info.bUseLargeFont = true;
	Info.bFireAndForget = false;
	Info.bAllowThrottleWhenFrameRateIsLow = false;
	
	// Code from Alessa Baker
	// https://codekitten.me/unreal/Notifications-In-Unreal-Engine_Part-2/
	/* 
		Create a Hyperlink that appears at the bottom right of the notification.
		A Hyperlink itself can call a function, in this case we create a small lambda to launch a URL.
		but you could call other functions here.
	*/
	Info.Hyperlink = FSimpleDelegate::CreateLambda([this]() {
		const FString DocsURL = TEXT("https://docs.unrealengine.com/");
		FPlatformProcess::LaunchURL(*DocsURL, nullptr, nullptr);
	});

	Info.HyperlinkText = LOCTEXT("ScreenshotPath", "Go to screenshot folder...");

	auto NotificationItem = FSlateNotificationManager::Get().AddNotification(Info);
	NotificationItem->SetCompletionState(SNotificationItem::CS_Success);
	NotificationItem->ExpireAndFadeout();
}

void FLevelShotModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FLevelShotCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FLevelShotCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLevelShotModule, LevelShot)
