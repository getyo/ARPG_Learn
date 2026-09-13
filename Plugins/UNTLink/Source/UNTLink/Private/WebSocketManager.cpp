#include "WebSocketManager.h"
#include "Editor/EditorEngine.h"
#include "UNTHandler.h"
#include "Misc/MessageDialog.h"

FWebSocketManager& FWebSocketManager::Get()
{
	static FWebSocketManager Instance;
	return Instance;
}

void FWebSocketManager::Initialize()
{
	
	const FString ServerAddress = TEXT("ws://127.0.0.1:56786");
    UE_LOG(LogTemp, Warning, TEXT("try connect unt server!"));
	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerAddress);

	WebSocket->OnConnected().AddRaw(this, &FWebSocketManager::OnConnected);
	WebSocket->OnConnectionError().AddRaw(this, &FWebSocketManager::OnConnectionError);
	WebSocket->OnClosed().AddRaw(this, &FWebSocketManager::OnConnectionClosed);
	WebSocket->OnMessage().AddRaw(this, &FWebSocketManager::OnMessageReceived);

	WebSocket->Connect();
}

void FWebSocketManager::Shutdown()
{
	if (GetEditorWorld())
	{
		GetEditorWorld()->GetTimerManager().ClearTimer(ReconnectTimerHandle);
		GetEditorWorld()->GetTimerManager().ClearTimer(HeartbeatTimerHandle);
	}

	if (WebSocket.IsValid())
	{
		WebSocket->Close();
		WebSocket.Reset();
	}
}

void FWebSocketManager::SendData(const FString& Data)
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Send(Data);
	}
	else
	{
		// 定义消息框的文本 
		FMessageDialog::Open( EAppMsgType::Ok,FText::FromString(TEXT("执行失败，请先启动虚幻盒子。") ));

		 
	}
}

void FWebSocketManager::OnConnected()
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket Connected"));
	// Implement any additional logic needed upon connection.
	UE_LOG(LogTemp, Warning, TEXT("Connected to WebSocket server!"));

	// 清除重连定时器，因为已经成功连接
	GetEditorWorld()->GetTimerManager().ClearTimer(ReconnectTimerHandle);

	FTimerDelegate HeartbeatDelegate;
	HeartbeatDelegate.BindLambda([this]() { SendHeartbeat(); });
	GetEditorWorld()->GetTimerManager().SetTimer(HeartbeatTimerHandle, HeartbeatDelegate, 10.0f, true);
    
}

void FWebSocketManager::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Error, TEXT("WebSocket Connection Error: %s"), *Error);
	// Implement reconnection logic or error handling as needed.
	ScheduleReconnect();
}

void FWebSocketManager::OnConnectionClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket Connection Closed: %s"), *Reason);
	// Implement reconnection logic or cleanup as needed.
	UE_LOG(LogTemp, Log, TEXT("Connection closed. Reconnecting in 5 seconds..."));
	GetEditorWorld()->GetTimerManager().ClearTimer(HeartbeatTimerHandle);
	ScheduleReconnect();
}

void FWebSocketManager::ScheduleReconnect()
{
	FTimerDelegate ReconnectDelegate;
	ReconnectDelegate.BindLambda([this]() { Initialize(); });
	GetEditorWorld()->GetTimerManager().SetTimer(ReconnectTimerHandle, ReconnectDelegate, 3.0f, false);
}

void FWebSocketManager::OnMessageReceived(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("WebSocket Message Received: %s"), *Message);
	// Process the received message.
	UE_LOG(LogTemp, Log, TEXT("Received: %s"), *Message);
	if (!MessageHandler)
	{
		MessageHandler = NewObject<UUNTHandler>();
	}

	MessageHandler->HandleMessage(Message);
	
}


UWorld* FWebSocketManager::GetEditorWorld() const
{
	if (GIsEditor)
	{
		UEditorEngine* EditorEngine = Cast<UEditorEngine>(GEngine);
		return EditorEngine ? EditorEngine->GetEditorWorldContext().World() : nullptr;
	}
	return nullptr;
}


void FWebSocketManager::SendHeartbeat()
{
	SendData(TEXT("{ \"Action\": \"PING\" }"));
}
 
 