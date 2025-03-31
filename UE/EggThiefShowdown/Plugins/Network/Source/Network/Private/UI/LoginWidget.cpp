// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWidget.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Channel/LoginChannel.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HaServerSubsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	if (!HaServerSubsystem->GetLoginChannel())
	{
		return;
	}
	HaServerSubsystem->GetLoginChannel()->OnCreateAccountResult.AddDynamic(this, &ThisClass::OnCreateAccountResult);
	HaServerSubsystem->GetLoginChannel()->OnLoginResult.AddDynamic(this, &ThisClass::OnLoginResult);

	UsernameTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnUsernameTextChanged);
	PasswordTextBox->OnTextChanged.AddDynamic(this, &ThisClass::OnPasswordTextChanged);
	PasswordTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnPasswordCommitted);
	PasswordTextBox->SetIsPassword(true);
	SendButton->OnClicked.AddDynamic(this, &ThisClass::OnSend);
}

void ULoginWidget::OnSend()
{
	if (GetUsername().IsEmpty() || GetPassword().IsEmpty()) { return; }

	if (IsCreateAccountChecked())
	{
		HaServerSubsystem->SendCreateAccount(GetUsername(), GetPassword());
	}
	else
	{
		HaServerSubsystem->SendLogin(GetUsername(), GetPassword());
	}

	CreateAccountCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	PasswordTextBox->SetText(FText());
}

void ULoginWidget::OnUsernameTextChanged(const FText& Text)
{
	FText ResultText = RemoveDisallowedString(Text.ToString());
	UsernameTextBox->SetText(ResultText);
}

void ULoginWidget::OnPasswordTextChanged(const FText& Text)
{
	FText ResultText = RemoveDisallowedString(Text.ToString());
	PasswordTextBox->SetText(ResultText);
}

void ULoginWidget::OnPasswordCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		OnSend();
	}
}

void ULoginWidget::SetAckMessage(const FText& Message)
{
	AckMessageBlock->SetText(Message);
}

FString ULoginWidget::GetUsername() const
{
	return UsernameTextBox->GetText().ToString();
}

FString ULoginWidget::GetPassword() const
{
	return PasswordTextBox->GetText().ToString();
}

bool ULoginWidget::IsCreateAccountChecked() const
{
	return CreateAccountCheckBox->IsChecked();
}

void ULoginWidget::OnConnectionStateChanged(EConnectionState NewConnectionState)
{
	if (LastConnectionState == NewConnectionState) { return; }

	LastConnectionState = NewConnectionState;

	switch (LastConnectionState)
	{
	case USOCK_Invalid:
		ServerState->SetFillColorAndOpacity(FLinearColor::Black);
		break;
	case USOCK_Closed:
		ServerState->SetFillColorAndOpacity(FLinearColor::Red);
		break;
	case USOCK_Pending:
		break;
	case USOCK_Open:
		ServerState->SetFillColorAndOpacity(FLinearColor::Green);
		break;
	default:
		break;
	}
}

void ULoginWidget::OnCreateAccountResult(uint32 ResultCode)
{
	switch (ResultCode)
	{
	case 0:
		SetAckMessage(FText::FromString(TEXT("Create Account Failed")));
		break;
	case 1:
		SetAckMessage(FText::FromString(TEXT("Create Account Success")));
		break;
	default:
		break;
	}
}

void ULoginWidget::OnLoginResult(uint32 ResultCode)
{
	/*enum ELoginResult : uint32
	{
		None,
		Success,
		UsernameNotFound,
		PasswordError,
	};*/

	switch (ResultCode)
	{
	case 1:
		SetAckMessage(FText::FromString(TEXT("Login Successed")));
		OnLogin.Broadcast(UserName);
		break;
	case 2:
		SetAckMessage(FText::FromString(TEXT("Username not found")));
		break;
	case 3:
		SetAckMessage(FText::FromString(TEXT("Password error")));
		break;
	default:
		break;
	}
}

FText ULoginWidget::RemoveDisallowedString(const FString& InString)
{
	if (InString.IsEmpty()) { return FText(); }

	FString String = InString;
	for (int32 i = 0; i < String.Len();)
	{
		TCHAR Char = String[i];
		if (
			!(
				(TEXT('A') <= Char && Char <= TEXT('Z')) ||
				(TEXT('a') <= Char && Char <= TEXT('z')) ||
				(TEXT('0') <= Char && Char <= TEXT('9'))
				)
			)
		{
			String.RemoveAt(i);
		}
		else
		{
			++i;
		}
	}

	{
		const int32 Length = String.Len();
		if (Length > UsernamePasswordMaxLength)
		{
			String = String.Left(UsernamePasswordMaxLength);
		}
	}

	return FText::FromString(String);
}

void ULoginWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	OnConnectionStateChanged(HaServerSubsystem->GetConnectionState());
}
