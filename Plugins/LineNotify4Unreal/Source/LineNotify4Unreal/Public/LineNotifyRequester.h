// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

enum EContentType
{
	CT_Text, CT_StampPackageID, CT_StampID, CT_ImageFile
};

struct FLineNotifyContent
{
private:
	// Content type
	EContentType ContentType;

	// The string to send
	FString StringData;

	// PNG format image data
	TArray<uint8> ImageData;

public:
	// Constructor - String Content
	FLineNotifyContent(EContentType ContentType, const FString& Content)
		: ContentType(ContentType)
		, StringData(Content)
	{
		ImageData.Empty();
	}

	// Constructor - Image Content
	FLineNotifyContent(const TArray<uint8>& Content, const FString& FileName)
		: ContentType(EContentType::CT_ImageFile)
		, StringData(FileName)
		, ImageData(Content)
	{
	}

	// Get the data to send
	TArray<uint8> GetContentArray();

private:
	// Get the number of bytes in a UTF8 string
	int GetUTF8StringSize(const FString& Text);
};

class LINENOTIFY4UNREAL_API ULineNotifyRequester
{
public:
	// Send content to LINE Notify
	static void SendContentToLINE(const FString& AccessToken, const TArray<FLineNotifyContent>& Contents);

private:
	// Called when the HTTP request is complete
	static void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
