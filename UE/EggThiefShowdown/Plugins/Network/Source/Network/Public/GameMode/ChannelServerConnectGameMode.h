// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChannelServerConnectGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginDelegate, int32, CurrentUserCount);

// 데디 서버가 채널 서버로 접속하는 GameMode
UCLASS(config = Network)
class NETWORK_API AChannelServerConnectGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	/**
	* PreLogin의 비동기 버전으로, 완료되었을 때 반드시 OnComplete를 호출해야 합니다. 그렇지 않으면 들어오는 클라이언트 연결이 중단되거나 시간 초과될 수 있습니다.
	* 이 기능은 서버가 게임 스레드를 차단하지 않고 로그인 자격 증명을 확인하기 위해 다른 서비스에 요청을 보낼 수 있도록 합니다.
	* 기본적으로, 이전 버전과의 호환성을 깨뜨리지 않기 위해 PreLogin()과 OnComplete를 즉시 호출합니다. 이를 재정의할 경우, PreLogin에서 실행된 체크를 반드시 실행하고,
	* GameModePreLoginEvent가 브로드캐스트되도록 해야 합니다.
	*/
	virtual void PreLoginAsync(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, const FOnPreLoginCompleteDelegate& OnComplete) override;
	
	/**
	 * 서버에 접속을 시도하는 플레이어를 수락하거나 거절합니다. ErrorMessage를 비어 있지 않은 문자열로 설정하면 로그인이 실패합니다.
	 * PreLogin은 Login이 호출되기 전에 호출됩니다. Login이 호출되기 전까지 상당한 게임 시간이 지날 수 있습니다.
	 *
	 * @param   Options                 플레이어가 전달한 URL 옵션(예: 이름/관전자 모드 등)
	 * @param   Address                 플레이어의 네트워크 주소
	 * @param   UniqueId                플레이어가 서버에 전달한 고유 ID
	 * @param   ErrorMessage            비어 있지 않은 값으로 설정되면, 해당 오류 메시지와 함께 플레이어의 접속이 거부됩니다.
	 */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Config)
	FString Host;

	UPROPERTY(Config)
	int32 Port;

protected:
	struct FPendingLogin
	{
		FPendingLogin(const FString& InOptions, const FString& InAddress,
			const FUniqueNetIdRepl& InUniqueId, const FOnPreLoginCompleteDelegate& InOnComplete,
			const FString& InUserName, const FString& InPassword)
			: Options(InOptions), Address(InAddress), UniqueId(InUniqueId), OnComplete(InOnComplete)
			, UserName(InUserName), Password(InPassword) { }

		const FString Options;
		const FString Address;
		const FUniqueNetIdRepl UniqueId;
		const FOnPreLoginCompleteDelegate OnComplete;
		const FString UserName;
		const FString Password;
	};

	UFUNCTION()
	void OnCheckAccountValidResultFunction(bool bResult, FString UserName, FString Password);
	
	UFUNCTION()
	void OnKickPlayerFunction(FString UserName);

	class UUEDediServerChannel* DediServerChannel = nullptr;
	TArray<FPendingLogin> PendingLogins;
	
	UPROPERTY(BlueprintAssignable)
	FOnLoginDelegate OnLoginDelegate;
};
