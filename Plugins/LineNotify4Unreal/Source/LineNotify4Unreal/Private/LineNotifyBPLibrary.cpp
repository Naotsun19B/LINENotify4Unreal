// Fill out your copyright notice in the Description page of Project Settings.

#include "LineNotifyBPLibrary.h"
#include "LineNotifyRequester.h"
#include "LineNotify4Unreal.h"
#include "Engine/GameViewportClient.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Misc/ConfigCacheIni.h"

FDelegateHandle ULineNotifyBPLibrary::OnScreenshotCapturedHandle;
FString ULineNotifyBPLibrary::TempAccessToken;
FString ULineNotifyBPLibrary::TempMessage;
bool ULineNotifyBPLibrary::TempSaveScreenshotLocally;

FString ULineNotifyBPLibrary::GetDefaultAccessToken()
{
	if (GConfig)
	{	
		FString AccessToken;
		if (GConfig->GetString(
			TEXT("/Script/LineNotify4UnrealEd.LineNotifySettings"),
			TEXT("AccessToken"),
			AccessToken,
			FPaths::Combine(FPaths::ProjectConfigDir(), TEXT("DefaultEditor.ini")))
			)
		{
			return AccessToken;
		}
	}

	return TEXT("Failed to get access token");
}

void ULineNotifyBPLibrary::SendMessage(const FString& AccessToken, const FString& Message)
{
	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));

	ULineNotifyRequester::SendContentToLINE(AccessToken, Contents);
}

void ULineNotifyBPLibrary::SendMessageAndStamp(const FString& AccessToken, const FString& Message, int StampPackageID, int StampID)
{
	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));
	Contents.Add(FLineNotifyContent(EContentType::CT_StampPackageID, FString::FromInt(StampPackageID)));
	Contents.Add(FLineNotifyContent(EContentType::CT_StampID, FString::FromInt(StampID)));

	ULineNotifyRequester::SendContentToLINE(AccessToken, Contents);
}

void ULineNotifyBPLibrary::SendMessageAndImageFile(const FString& AccessToken, const FString& Message, const FString& FilePath)
{
	TArray<uint8> ImageData;
	if (FFileHelper::LoadFileToArray(ImageData, *FilePath))
	{
		TArray<FLineNotifyContent> Contents;
		Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));
		Contents.Add(FLineNotifyContent(ImageData, FPaths::GetCleanFilename(FilePath)));

		ULineNotifyRequester::SendContentToLINE(AccessToken, Contents);
	}
	else
	{
		UE_LOG(LineNotify4Unreal, Warning, TEXT("The specified file was not found ..."));
	}
}

void ULineNotifyBPLibrary::SendMessageAndTexture(const FString& AccessToken, const FString& Message, UTexture2D* Texture)
{
	if (Texture == nullptr)
	{
		UE_LOG(LineNotify4Unreal, Warning, TEXT("No texture is specified for sending"));
		return;
	}

	FString FileName = TEXT("Texture_") + FDateTime::Now().ToString() + TEXT(".png");
	FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), FileName));

	Texture->UpdateResource();
	FTexture2DMipMap* MM = &Texture->PlatformData->Mips[0];

	TArray<FColor> Bitmap;
	int Width = MM->SizeX;
	int Height = MM->SizeY;

	Bitmap.InsertZeroed(0, Width * Height);

	FByteBulkData* RawImageData = &MM->BulkData;

	FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));

	for (int i = 0; i < (Width * Height); ++i)
	{
		Bitmap[i] = FormatedImageData[i];
		Bitmap[i].A = 255;
	}

	RawImageData->Unlock();

	TArray<uint8> ImageData = ConvertBitmapToPngImage(Width, Height, Bitmap);

	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, Message));
	Contents.Add(FLineNotifyContent(ImageData, FileName));

	ULineNotifyRequester::SendContentToLINE(AccessToken, Contents);
}

void ULineNotifyBPLibrary::SendMessageAndScreenshot(const FString& AccessToken, const FString& Message, bool bInShownUI, bool bInsertTimeStamp, bool SaveScreenshotLocally)
{
	TempAccessToken = AccessToken;
	TempSaveScreenshotLocally = SaveScreenshotLocally;
	
	if (bInsertTimeStamp)
	{
		TempMessage = TEXT("[") + FDateTime::Now().ToString() + TEXT("] ") + Message;
	}
	else
	{
		TempMessage = Message;
	}

	//スクリーンショット時のイベントをバインド
	if (!UGameViewportClient::OnScreenshotCaptured().IsBound())
	{
		OnScreenshotCapturedHandle = UGameViewportClient::OnScreenshotCaptured().AddStatic(&ULineNotifyBPLibrary::OnScreenshotCaptured);
	}

	FScreenshotRequest::RequestScreenshot(bInShownUI);
}

void ULineNotifyBPLibrary::OnScreenshotCaptured(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
{
	FString FileName = TEXT("Screenshot_") + FDateTime::Now().ToString() + TEXT(".png");

	TArray<uint8> ImageData = ConvertBitmapToPngImage(Width, Height, Bitmap);

	TArray<FLineNotifyContent> Contents;
	Contents.Add(FLineNotifyContent(EContentType::CT_Text, TempMessage));
	Contents.Add(FLineNotifyContent(ImageData, FileName));

	ULineNotifyRequester::SendContentToLINE(TempAccessToken, Contents);

	if (TempSaveScreenshotLocally)
	{
		FString FilePath = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Screenshots"), TEXT("LineNotify4Unreal"), FileName));
		if (!FFileHelper::SaveArrayToFile(ImageData, *FilePath))
		{
			UE_LOG(LineNotify4Unreal, Warning, TEXT("Failed to save screenshot ..."));
		}
	}

	TempAccessToken = TEXT("");
	TempMessage = TEXT("");
	TempSaveScreenshotLocally = false;

	//スクリーンショット時のイベントのバインドを解除
	UGameViewportClient::OnScreenshotCaptured().Remove(OnScreenshotCapturedHandle);
}

TArray<uint8> ULineNotifyBPLibrary::ConvertBitmapToPngImage(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
{
	TArray<uint8> PngData;
	TArray<FColor> BitmapCopy(Bitmap);
	FImageUtils::CompressImageArray(Width, Height, BitmapCopy, PngData);

	return PngData;
}
