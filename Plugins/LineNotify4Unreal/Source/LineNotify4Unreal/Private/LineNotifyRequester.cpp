// Fill out your copyright notice in the Description page of Project Settings.

#include "LineNotifyRequester.h"
#include "LineNotify4Unreal.h"
#include "HttpModule.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

TArray<uint8> FLineNotifyContent::GetContentArray()
{
	TArray<uint8> ContentData;

	//文字列データを作成
	if (ContentType != EContentType::CT_ImageFile)
	{
		FString ContentName;
		switch (ContentType)
		{
		case CT_Text:			
			ContentName = TEXT("message");			
			break;
		case CT_StampPackageID: 
			ContentName = TEXT("stickerPackageId"); 
			break;
		case CT_StampID:		
			ContentName = TEXT("stickerId");		
			break;
		default: 
			break;
		}

		FString Content = TEXT("Content-Disposition: form-data; name=\"") + ContentName + TEXT("\"\r\n\r\n") + StringData;
		ContentData.Append((uint8*)TCHAR_TO_UTF8(*Content), GetUTF8StringSize(Content));
	}
	//画像データを作成
	else
	{
		FString FileHeader = TEXT("Content-Disposition: form-data;name=\"imageFile\";");
		FileHeader.Append(TEXT("filename=\"") + StringData + TEXT("\"\r\n"));
		FileHeader.Append("Content-Type: image/png\r\n\r\n");
		ContentData.Append((uint8*)TCHAR_TO_UTF8(*FileHeader), GetUTF8StringSize(FileHeader));
		ContentData.Append(ImageData);
	}
	
	return ContentData;
}

int FLineNotifyContent::GetUTF8StringSize(const FString& Text)
{
	{
		int Size = 0;

		for (TCHAR Char : Text)
		{
			const char* Temp = TCHAR_TO_UTF8(*FString::Chr(Char));
			uint8 Code = static_cast<uint8>(*Temp);

			if ((Code >= 0x00) && (Code <= 0x7f))
			{
				Size += 1;
			}
			else if ((Code >= 0xc2) && (Code <= 0xdf))
			{
				Size += 2;
			}
			else if ((Code >= 0xe0) && (Code <= 0xef))
			{
				Size += 3;
			}
			else if ((Code >= 0xf0) && (Code <= 0xf7))
			{
				Size += 4;
			}
			else if ((Code >= 0xf8) && (Code <= 0xfb))
			{
				Size += 5;
			}
			else if ((Code >= 0xfc) && (Code <= 0xfd))
			{
				Size += 6;
			}
		}

		return Size;
	}
}

void ULineNotifyRequester::SendContentToLINE(const FString& AccessToken, const TArray<FLineNotifyContent>& Contents)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(TEXT("https://notify-api.line.me/api/notify"));
	Request->SetVerb(TEXT("POST"));

	//ヘッダーを設定
	FString Boundary = "---------------------------" + FString::FromInt(FDateTime::Now().GetTicks());
	Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary =" + Boundary));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AccessToken);
	
	//送信するデータを作成
	TArray<uint8> RequestContent;
	for (auto Content : Contents)
	{
		FString BeginBoundry = "\r\n--" + Boundary + "\r\n";
		RequestContent.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundry), BeginBoundry.Len());

		RequestContent.Append(Content.GetContentArray());
	}

	FString EndBoundary = "\r\n--" + Boundary + "--\r\n";
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

	Request->SetContent(RequestContent);

	Request->OnProcessRequestComplete().BindStatic(&ULineNotifyRequester::OnResponseReceived);

	Request->ProcessRequest();
}

void ULineNotifyRequester::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//接続成功
	if (bWasSuccessful) {
		//通信成功
		if (Response->GetResponseCode() >= 200 && Response->GetResponseCode() < 300)
		{
			UE_LOG(LineNotify4Unreal, Log, TEXT("Communication success !\n[HTTP Status Code] %d"), Response->GetResponseCode());
		}
		//通信失敗
		else
		{
			FString ErrorMessage = Response->GetContentAsString();

			const auto JsonReader = TJsonReaderFactory<TCHAR>::Create(ErrorMessage);
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				ErrorMessage = JsonObject->GetStringField(TEXT("message"));
			}
			UE_LOG(LineNotify4Unreal, Warning, TEXT("Communication failure ...\n[HTTP Status Code] %d\n[Error Message] %s"), Response->GetResponseCode(), *ErrorMessage);
		}
	}
	//接続失敗
	else
	{
		UE_LOG(LineNotify4Unreal, Error, TEXT("Failed to connect ..."));
	}
}