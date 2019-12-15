// Fill out your copyright notice in the Description page of Project Settings.

#include "LineNotifySettings.h"

ULineNotifySettings::ULineNotifySettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, AccessToken(TEXT("Enter your access token obtained from the LINE Notify web page."))
	, UseCompilationNotification(true)
	, SendDumpedLogs(true)
	, CompileSucceeded(TEXT("Compilation succeeded"))
	, CompileCanceled(TEXT("Build was canceled"))
	, CompileUpToDate(TEXT("All targets were up to date, used only with -canskiplink"))
	, CompileCrashOrAssert(TEXT("The process has most likely crashed. This is what UE returns in case of an assert"))
	, CompileFailedDueToHeaderChange(TEXT("Compilation failed because generated code changed which was not supported"))
	, CompileOtherCompilationError(TEXT("Compilation failed due to compilation errors"))
	, CompileUnsupported(TEXT("Compilation is not supported in the current build"))
	, CompileUnknown(TEXT("Unknown error"))
	, UseLightBuildNotification(true)
	, LightBuildSucceeded(TEXT("Light build succeeded"))
	, LightBuildFailed(TEXT("Light build failed"))
	, AddStampToNotification(false)
	, SucceededStickerPackageID(1)
	, SucceededStickerID(106)
	, FailedStickerPackageID(1)
	, FailedStickerID(9)
{
}
