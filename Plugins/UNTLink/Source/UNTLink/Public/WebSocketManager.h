#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "Modules/ModuleManager.h" 
#include "UNTHandler.h"
#include "WebSocketsModule.h"

/**
 * Global WebSocket manager class for managing WebSocket connections and communications.
 */
class FWebSocketManager
{
public:
	// Gets the singleton instance of the WebSocket manager.
	static FWebSocketManager& Get();

	// Initializes the WebSocket connection.
	void Initialize();

	// Closes the WebSocket connection and cleans up resources.
	void Shutdown();

	// Sends data through the WebSocket connection.
	void SendData(const FString& Data);

	
	UWorld* GetEditorWorld() const;
	void SendHeartbeat();

private:
	// Private constructor for singleton pattern.
	FWebSocketManager() {}

	// The WebSocket connection.
	TSharedPtr<IWebSocket> WebSocket; 
	FTimerHandle ReconnectTimerHandle;
	FTimerHandle HeartbeatTimerHandle;

	
	UUNTHandler* MessageHandler;

	// Handles the "Connected" event.
	void OnConnected();

	// Handles the "ConnectionError" event.
	void OnConnectionError(const FString& Error);

	// Handles the "Closed" event.
	void OnConnectionClosed(int32 StatusCode, const FString& Reason, bool bWasClean);

	// Handles the "MessageReceived" event.
	void OnMessageReceived(const FString& Message);

	 
	
	
	void ScheduleReconnect();
};
