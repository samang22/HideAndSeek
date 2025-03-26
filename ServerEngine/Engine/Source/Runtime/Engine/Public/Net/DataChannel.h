#pragma once
#include "CoreMinimal.h"
#include "Engine/NetConnection.h"
#include "Engine/Channel.h"
#include "DataBunch.h"

template<uint8 MessageType> class FNetControlMessage
{
};

#define DEFINE_CONTROL_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Control, 0, Name, Index)

DEFINE_CONTROL_CHANNEL_MESSAGE(Hello, 0);