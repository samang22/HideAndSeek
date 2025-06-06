#pragma once

// Core Module이 아닌 곳에서는 CoreMinimal을 추가하여 사용한다.

#include "CoreGlobals.h"

#include "GenericPlatform/GenericPlatformTime.h"

#include "Containers/JobQueue.h"
#include "Containers/BufferedContainer.h"

#include "UObject/EnginePtr.h"
#include "UObject/Object.h"
#include "UObject/Class.h"

#include "Delegate/Delegate.h"

#include "Modules/ModuleManager.h"

#include "Logging/Logger.h"

#include "Math/SimpleMath.h"
#include "Math/Transform.h"
#include "Math/Rotator.h"

#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include "Misc/ConfigCacheIni.h"
#include "Misc/Names.h"
#include "Misc/Paths.h"
#include "Misc/App.h"

#include "Templates/SubclassOf.h"
#include "Templates/RefCounting.h"
#include "Templates/AlignmentTemplates.h"

#include "Serialization/Archive.h"

#include "Task/Task.h"