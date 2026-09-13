#include "UNTHandler.h"
#include "Engine/Engine.h"
#include "WebSocketBlueprintLibrary.h" // 确保包含了WebSocketManager的头文件


void UUNTHandler::HandleMessage(const FString& Message)
{
    if (Message.StartsWith(TEXT("cmd ")))
    {
        // 去掉"cmd "前缀，并执行控制台命令
        FString Command = Message.Mid(4);
        if (GEngine)
        {
            GEngine->Exec(GEngine->GetWorld(), *Command);
        }
    }
    else if (Message.StartsWith(TEXT("log ")))
    {
        // 去掉"log "前缀，并打印日志
        FString LogMessage = Message.Mid(4);
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
    }
    // 在这里，你可以继续扩展以处理其他消息前缀
 
}
