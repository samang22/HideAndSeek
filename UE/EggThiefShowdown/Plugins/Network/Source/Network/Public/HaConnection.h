// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IpConnection.h"
#include "HaConnection.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_API UHaConnection : public UIpConnection
{
	GENERATED_BODY()
public:
	UHaConnection();
};
