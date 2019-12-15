// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "LineNotifyBPLibrary.generated.h"

UCLASS()
class LINENOTIFY4UNREAL_API ULineNotifyBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	// Handle of registered event
	static FDelegateHandle OnScreenshotCapturedHandle;

	// Variable for temporary storage during screenshot
	static FString TempAccessToken;
	static FString TempMessage;
	static bool TempSaveScreenshotLocally;

public:
	// Get the access token specified in the project settings
	UFUNCTION(BlueprintPure, Category = "LineNotify4Unral", meta = (DevelopmentOnly))
	static FString GetDefaultAccessToken();

	// Send a text to LINE
	UFUNCTION(BlueprintCallable, Category = "LineNotify4Unreal")
	static void SendMessage(const FString& AccessToken, const FString& Message);

	// Send text and stamp to LINE
	UFUNCTION(BlueprintCallable, Category = "LineNotify4Unreal")
	static void SendMessageAndStamp(const FString& AccessToken, const FString& Message, int StampPackageID, int StampID);

	// Send text and image file to LINE
	UFUNCTION(BlueprintCallable, Category = "LineNotify4Unreal")
	static void SendMessageAndImageFile(const FString& AccessToken, const FString& Message, const FString& FilePath);

	// Send text and UTexture2D to LINE
	UFUNCTION(BlueprintCallable, Category = "LineNotify4Unreal")
	static void SendMessageAndTexture(const FString& AccessToken, const FString& Message, UTexture2D* Texture);

	// Send text and screenshot to LINE
	UFUNCTION(BlueprintCallable, Category = "LineNotify4Unreal", meta = (AdvancedDisplay = 2))
	static void SendMessageAndScreenshot(const FString& AccessToken, const FString& Message, bool bInShownUI, bool bInsertTimeStamp, bool SaveScreenshotLocally);

private:
	// Called when a screenshot is taken
	static void OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap);

public:
	// Convert bitmap data to PNG image data
	static TArray<uint8> ConvertBitmapToPngImage(int32 Width, int32 Height, const TArray<FColor>& Bitmap);
};
