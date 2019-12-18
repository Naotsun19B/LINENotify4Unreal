// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LineNotifySettings.generated.h"

UCLASS(config = Editor, defaultconfig)
class LINENOTIFY4UNREALED_API ULineNotifySettings : public UObject
{
	GENERATED_UCLASS_BODY()
	
public:
	// Enter your access token obtained from the LINE Notify web page
	UPROPERTY(EditAnywhere, Category = "AccessToken", Config = Category)
	FString AccessToken;
	
	// Use notification when compilation is complete
	UPROPERTY(EditAnywhere, Category = "CompileNotification", Config = Category)
	bool bUseCompilationNotification;

	// Whether to add the project name to the compilation result message
	UPROPERTY(EditAnywhere, Category = "CompileNotification", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Insert Project Name"))
	bool bInsertProjectNameInCompilationNotification;

	// Send dump log with compilation results
	UPROPERTY(EditAnywhere, Category = "CompileNotification", Config = Category, meta = (EditCondition = "bUseCompilationNotification"))
	bool bSendDumpedLogs;

	// Compilation succeeded
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Succeeded"))
	FString CompileSucceeded;

	// Build was canceled
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Canceled"))
	FString CompileCanceled;

	// All targets were up to date, used only with -canskiplink
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Up To Date"))
	FString CompileUpToDate;

	// The process has most likely crashed. This is what UE returns in case of an assert
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Crash Or Assert"))
	FString CompileCrashOrAssert;

	// Compilation failed because generated code changed which was not supported
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Failed Due To Header Change"))
	FString CompileFailedDueToHeaderChange;

	// Compilation failed due to compilation errors
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Other Compilation Error"))
	FString CompileOtherCompilationError;

	// Compilation is not supported in the current build
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Unsupported"))
	FString CompileUnsupported;

	// Unknown error
	UPROPERTY(EditAnywhere, Category = "CompileNotification|Compilation Result Message", Config = Category, meta = (EditCondition = "bUseCompilationNotification", DisplayName = "Unknown"))
	FString CompileUnknown;

	// Use notification when light build is complete
	UPROPERTY(EditAnywhere, Category = "LightBuildNotification", Config = Category)
	bool bUseLightBuildNotification;
	
	// Whether to add the project name to the light build result message
	UPROPERTY(EditAnywhere, Category = "LightBuildNotification", Config = Category, meta = (EditCondition = "bUseLightBuildNotification", DisplayName = "Insert Project Name"))
	bool bInsertProjectNameInLightBuildNotification;

	// Message sent when light build is successful
	UPROPERTY(EditAnywhere, Category = "LightBuildNotification|Light Build Result Message", Config = Category, meta = (EditCondition = "bUseLightBuildNotification", DisplayName = "Succeeded"))
	FString LightBuildSucceeded;
	
	// Message sent when light build fails
	UPROPERTY(EditAnywhere, Category = "LightBuildNotification|Light Build Result Message", Config = Category, meta = (EditCondition = "bUseLightBuildNotification", DisplayName = "Failed"))
	FString LightBuildFailed;

	// Add a stamp to the notification at the end of compilation and at the end of light build
	UPROPERTY(EditAnywhere, Category = "Stamp", Config = Category)
	bool bAddStampToNotification;
	
	// Package ID of stamp
	UPROPERTY(EditAnywhere, Category = "Stamp|Succeeded", Config = Category, meta = (EditCondition = "bAddStampToNotification", DisplayName = "Sticker Package ID"))
	int SucceededStickerPackageID;

	// ID of the stamp in the package
	UPROPERTY(EditAnywhere, Category = "Stamp|Succeeded", Config = Category, meta = (EditCondition = "bAddStampToNotification", DisplayName = "Sticker ID"))
	int SucceededStickerID;

	// Package ID of stamp
	UPROPERTY(EditAnywhere, Category = "Stamp|Failed", Config = Category, meta = (EditCondition = "bAddStampToNotification", DisplayName = "Sticker Package ID"))
	int FailedStickerPackageID;

	// ID of the stamp in the package
	UPROPERTY(EditAnywhere, Category = "Stamp|Failed", Config = Category, meta = (EditCondition = "bAddStampToNotification", DisplayName = "Sticker ID"))
	int FailedStickerID;
};
