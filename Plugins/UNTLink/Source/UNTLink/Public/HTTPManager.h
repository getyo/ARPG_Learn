#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

class FHTTPManager
{
public:
	static FHTTPManager& Get();

	void Initialize();
	void Shutdown();

	void SendData(const FString& Data);
	void ProcessResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FHTTPManager() {}
	FHTTPManager(const FHTTPManager&) = delete;
	FHTTPManager& operator=(const FHTTPManager&) = delete;

	FHttpModule* HttpModule;
	FString ServerURL;
};
