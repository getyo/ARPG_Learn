#include "HTTPManager.h"
#include "HttpModule.h"
#include "Misc/MessageDialog.h"

FHTTPManager& FHTTPManager::Get()
{
	static FHTTPManager Instance;
	return Instance;
}

void FHTTPManager::Initialize()
{
	HttpModule = &FHttpModule::Get();
	ServerURL = TEXT("http://127.0.0.1:46786/data");
}

void FHTTPManager::Shutdown()
{
	// 资源释放逻辑（如有需要）
}

void FHTTPManager::SendData(const FString& Data)
{
	if (!HttpModule)
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Module not initialized"));
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule->CreateRequest();
	Request->OnProcessRequestComplete().BindRaw(this, &FHTTPManager::ProcessResponse);
	Request->SetURL(ServerURL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(Data);

	// 设置超时时间，避免长时间等待无响应的情况
	Request->SetTimeout(5.0f);  // 10秒超时，可根据需求调整

	// 检查 URL 和数据有效性
	if (Request->GetURL().IsEmpty() || Data.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid URL or empty data"));
		return;
	}

	// 请求处理失败时的提示
	if (!Request->ProcessRequest())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("导入失败，请确认虚幻盒子已启动。")));
		UE_LOG(LogTemp, Error, TEXT("HTTP Request failed to process"));
	}
}

void FHTTPManager::ProcessResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
	{
		// 检查状态码是否为 200 OK
		if (Response->GetResponseCode() == 200)
		{
			UE_LOG(LogTemp, Log, TEXT("HTTP Response: %s"), *Response->GetContentAsString());
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("导入失败，请确认虚幻盒子已启动。")));
			UE_LOG(LogTemp, Warning, TEXT("Unexpected HTTP Response Code: %d"), Response->GetResponseCode());
		}
	}
	else
	{
		// 处理连接失败或无效响应的情况
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("导入失败，请确认虚幻盒子已启动。")));
		UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed: No valid response or connection timeout."));
	}
}
