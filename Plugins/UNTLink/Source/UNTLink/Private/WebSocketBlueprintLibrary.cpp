#include "WebSocketBlueprintLibrary.h"
#include "WebSocketManager.h" // 确保包含了WebSocketManager的头文件

void UWebSocketBlueprintLibrary::SendWebSocketData(const FString& Data)
{
	FWebSocketManager::Get().SendData(Data); // 调用WebSocket管理器发送数据
}
