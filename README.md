# LINE Notify 4 Unreal
![Plugin](https://user-images.githubusercontent.com/51815450/70860404-a6920700-1f64-11ea-8a6a-6b7aa7d25538.PNG)

LINE Notifyを使用してメッセージ、スタンプ、画像、テクスチャを送信するブループリントノードを追加します。  
さらに、コンパイル終了時とライトビルド終了時に通知する機能も追加します。  

このプラグインを使用するには事前にLINE Notifyのアクセストークンを取得する必要があります。  
LINE Notifyの公式ページより取得できます。  
https://notify-bot.line.me/ja/  

# 動作環境
UE4.20 ～ UE4.23  

UE4.22以外のバージョンでテクスチャが正常に送信されないことがあります。

# ブループリントノードの使い方  
**全てのSend~ノードのAccess Tokenには取得したアクセストークンを指定してください。**  
**また、同じくMessageには送信する文字列を指定してください。**  
**空の文字列を指定すると正常に送信されません。**

## SendMessage  
![SendMessage](https://user-images.githubusercontent.com/51815450/70860122-cc1d1180-1f60-11ea-8553-1dcab4791f2a.jpg)

## SendMessageAndStamp  
![SendMessageAndStamp](https://user-images.githubusercontent.com/51815450/70860127-d8a16a00-1f60-11ea-85bf-da7cd01b63bf.jpg)

Sticker Package IDとSticker IDは以下のページを参考にしてください。  
https://devdocs.line.me/files/sticker_list.pdf  

## SendMessageAndImageFile  
![SendMessageAndImageFile](https://user-images.githubusercontent.com/51815450/70860146-1bfbd880-1f61-11ea-807e-d016dd90ebe4.jpg)

File Pathには送信したい画像のファイルパスを指定してください。  
ファイルピッカープラグインも公開しているのでよろしければご利用ください。  
https://github.com/Naotsun19B/FilePicker

## SendMessageAndTexture  
![SendMessageAndTexture](https://user-images.githubusercontent.com/51815450/70860160-61b8a100-1f61-11ea-8413-57b84d517ee1.jpg)  

Textureには送信したいテクスチャを指定してください。  
Download Imageノードと併用するとインターネット上の画像も送信できます。  
https://docs.unrealengine.com/en-US/BlueprintAPI/Class/AsyncTaskDownloadImage/DownloadImage/index.html

## SendMessageAndScreenshot  
![SendMessageAndScreenshot](https://user-images.githubusercontent.com/51815450/70860181-bf4ced80-1f61-11ea-9c26-8547e775d189.jpg)

In Shown UIはスクリーンショットにUIを含めるかのフラグです。  
Insert Time Stampは送信するメッセージにスクリーンショットを撮った時間を付加するかのフラグです。
Save Screen ShotLocallyは撮ったスクリーンショットを保存するかのフラグです。  
保存先は [Project Dir] - [Saved] - [Screenshots] - [LineNotiify4Unreal] です。

## GetDefaultAccessToken  
![GetDefaultAccessToken](https://user-images.githubusercontent.com/51815450/70860278-333bc580-1f63-11ea-9bf5-c9ebec2cbb69.jpg)

プロジェクト設定のAccessTokenに設定されているアクセストークンを取得します。

# プロジェクト設定  
![ProjectSettings](https://user-images.githubusercontent.com/51815450/70860296-68e0ae80-1f63-11ea-9f89-2bc8e0d3a22d.jpg)

Access Token - コンパイル終了時とライトビルド終了時に行われる通知で使用するアクセストークンを指定してください。  

Use Complilation Notification - コンパイル終了時に結果を通知するかのフラグです。  
Send Dumped Logs - コンパイル結果と共にログも送信するかのフラグです。  
Compilation Result Message - コンパイル結果ごとの送信するメッセージを指定します。  

Use Light Build Notification - ライトビルド終了時に結果を通知するかのフラグです。  
Light Build Result Message - ライトビルド結果ごとの送信するメッセージを指定します。  

Add Stamp to Notification - コンパイル終了時とライトビルド終了時の通知にスタンプを付加するかのフラグです。  
Succeeded - コンパイルとライトビルドが成功した際に送信されるスタンプのSticker Package IDとSticker IDを指定します。  
Failed - コンパイルとライトビルドが失敗した際に送信されるスタンプのSticker Package IDとSticker IDを指定します。  

