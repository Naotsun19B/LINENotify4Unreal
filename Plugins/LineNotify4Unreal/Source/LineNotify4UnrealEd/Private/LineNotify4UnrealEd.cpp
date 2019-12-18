// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LineNotify4UnrealEd.h"
#include "LineNotifyRequester.h"
#include "LineNotifySettings.h"
#include "ISettingsModule.h"
#include "Editor.h"
#include "Engine/GameViewportClient.h"
#include "Misc/HotReloadInterface.h"
#include "Misc/DateTime.h"
#include "Misc/App.h"

#define LOCTEXT_NAMESPACE "FLineNotify4UnrealEdModule"

void FLineNotify4UnrealEdModule::StartupModule()
{
	//�v���W�F�N�g�ݒ�ɓo�^
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"LINE Notify 4 Unreal",
			LOCTEXT("PluginName", "LINE Notify 4 Unreal"),
			LOCTEXT("PluginDescription", "You can set the access token of LINE Notify 4 Unreal and various notification contents"),
			GetMutableDefault<ULineNotifySettings>()
		);
	}

	//�R���p�C���I�����̃C�x���g���o�C���h
	const auto& HotReloadModule = IHotReloadInterface::GetPtr();
	OnCompileFinishedHandle = HotReloadModule->OnModuleCompilerFinished().AddRaw(this, &FLineNotify4UnrealEdModule::OnCompileFinished);

	//���C�g�r���h�I�����̃C�x���g���o�C���h
	OnLightBuildSucceededHandle = FEditorDelegates::OnLightingBuildSucceeded.AddRaw(this, &FLineNotify4UnrealEdModule::OnLightBuildSucceeded);
	OnLightBuildFailedHandle = FEditorDelegates::OnLightingBuildFailed.AddRaw(this, &FLineNotify4UnrealEdModule::OnLightBuildFailed);
}

void FLineNotify4UnrealEdModule::ShutdownModule()
{
	//�v���W�F�N�g�ݒ�̓o�^������
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->UnregisterSettings(
			"Project",
			"Plugins",
			"LINE Notify 4 Unreal"
		);
	}

	//�R���p�C���I�����̃C�x���g�̃o�C���h������
	const auto& HotReloadModule = IHotReloadInterface::GetPtr();
	HotReloadModule->OnModuleCompilerFinished().Remove(OnCompileFinishedHandle);

	//���C�g�r���h�I�����̃C�x���g�̃o�C���h������
	FEditorDelegates::OnLightingBuildSucceeded.Remove(OnLightBuildSucceededHandle);
	FEditorDelegates::OnLightingBuildFailed.Remove(OnLightBuildFailedHandle);
}

void FLineNotify4UnrealEdModule::OnCompileFinished(const FString& LogDump, ECompilationResult::Type CompilationResult, bool bShowLog) const
{
	const ULineNotifySettings* Settings = GetDefault<ULineNotifySettings>();

	if (!Settings->bUseCompilationNotification)
	{
		return;
	}

	FString Message;

	if (Settings->bInsertProjectNameInCompilationNotification)
	{
		Message += TEXT("<") + FString(FApp::GetProjectName(), 64) + TEXT("> ");
	}

	switch (CompilationResult)
	{
	case ECompilationResult::Succeeded:
		Message = Settings->CompileSucceeded;
		break;
	case ECompilationResult::Canceled:
		Message = Settings->CompileCanceled;
		break;
	case ECompilationResult::UpToDate:
		Message = Settings->CompileUpToDate;
		break;
	case ECompilationResult::CrashOrAssert:
		Message = Settings->CompileCrashOrAssert;
		break;
	case ECompilationResult::FailedDueToHeaderChange:
		Message = Settings->CompileFailedDueToHeaderChange;
		break;
	case ECompilationResult::OtherCompilationError:
		Message = Settings->CompileOtherCompilationError;
		break;
	case ECompilationResult::Unsupported:
		Message = Settings->CompileUnsupported;
		break;
	default:
		Message = Settings->CompileUnknown;
		break;
	}

	if (Settings->bSendDumpedLogs)
	{
		Message += TEXT("\n\n[Output Log]\n") + LogDump;
	}

	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));

	if (Settings->bAddStampToNotification)
	{
		if (CompilationResult == ECompilationResult::Succeeded)
		{
			Contents.Add(FLineNotifyContent(EContentType::CT_StampPackageID, FString::FromInt(Settings->SucceededStickerPackageID)));
			Contents.Add(FLineNotifyContent(EContentType::CT_StampID, FString::FromInt(Settings->SucceededStickerID)));
		}
		else
		{
			Contents.Add(FLineNotifyContent(EContentType::CT_StampPackageID, FString::FromInt(Settings->FailedStickerPackageID)));
			Contents.Add(FLineNotifyContent(EContentType::CT_StampID, FString::FromInt(Settings->FailedStickerID)));
		}
	}
	
	ULineNotifyRequester::SendContentToLINE(Settings->AccessToken, Contents);
}

void FLineNotify4UnrealEdModule::OnLightBuildSucceeded()
{
	const ULineNotifySettings* Settings = GetDefault<ULineNotifySettings>();

	if (!Settings->bUseLightBuildNotification)
	{
		return;
	}

	FString Message;

	if (Settings->bInsertProjectNameInLightBuildNotification)
	{
		Message += TEXT("<") + FString(FApp::GetProjectName(), 64) + TEXT("> ");
	}

	Message += Settings->LightBuildSucceeded;

	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));

	if (Settings->bAddStampToNotification)
	{
		Contents.Add(FLineNotifyContent(EContentType::CT_StampPackageID, FString::FromInt(Settings->SucceededStickerPackageID)));
		Contents.Add(FLineNotifyContent(EContentType::CT_StampID, FString::FromInt(Settings->SucceededStickerID)));
	}

	ULineNotifyRequester::SendContentToLINE(Settings->AccessToken, Contents);
}

void FLineNotify4UnrealEdModule::OnLightBuildFailed()
{
	const ULineNotifySettings* Settings = GetDefault<ULineNotifySettings>();

	if (!Settings->bUseLightBuildNotification)
	{
		return;
	}

	FString Message;

	if (Settings->bInsertProjectNameInLightBuildNotification)
	{
		Message += TEXT("<") + FString(FApp::GetProjectName(), 64) + TEXT("> ");
	}

	Message += Settings->LightBuildFailed;

	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));

	if (Settings->bAddStampToNotification)
	{
		Contents.Add(FLineNotifyContent(EContentType::CT_StampPackageID, FString::FromInt(Settings->FailedStickerPackageID)));
		Contents.Add(FLineNotifyContent(EContentType::CT_StampID, FString::FromInt(Settings->FailedStickerID)));
	}

	ULineNotifyRequester::SendContentToLINE(Settings->AccessToken, Contents);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLineNotify4UnrealEdModule, LineNotify4UnrealEd)
