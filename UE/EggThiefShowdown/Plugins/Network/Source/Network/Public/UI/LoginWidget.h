// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "LoginWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSendButtonClicked, FString, UserName, FString, Password, bool, bCreateAccount);


/**
 *
 */
UCLASS()
class NETWORK_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnSend();
	UFUNCTION()
	void OnUsernameTextChanged(const FText& Text);
	UFUNCTION()
	void OnPasswordTextChanged(const FText& Text);
	UFUNCTION()
	void OnPasswordCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void SetAckMessage(const FText& Message);
	FString GetUsername() const;
	FString GetPassword() const;
	bool IsCreateAccountChecked() const;

	void SetMaxLength(int32 InLength) { UsernamePasswordMaxLength = InLength; }

	void OnConnectionStateChanged(EConnectionState NewConnectionState);

public:
	UFUNCTION()
	void OnCreateAccountResult(uint32 ResultCode);
	UFUNCTION()
	void OnLoginResult(uint32 ResultCode);

protected:
	FText RemoveDisallowedString(const FString& InString);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ServerState;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* UsernameTextBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* PasswordTextBox;

	UPROPERTY(meta = (BindWidget))
	UButton* SendButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AckMessageBlock;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CreateAccountCheckBox;

	int32 UsernamePasswordMaxLength = 15;

protected:
	class UHaServerSubsystem* HaServerSubsystem = nullptr;
	EConnectionState LastConnectionState = EConnectionState::USOCK_Invalid;
};
