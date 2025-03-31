#include "LaunchEngineLoop.h"
#include "EngineModule.h"
#include "Engine/Engine.h"

CORE_API map<FString, UClass*>& GetClassMap(); 

FEngineLoop::~FEngineLoop()
{
	FModuleManager::Get(true);
}

int32 FEngineLoop::PreInit(const TCHAR* CmdLine)
{
	FLogger::Get();
	FModuleManager::Get();
	FTaskManager::Get();
	FConfigCacheIni::Get();

	// PreInitPreStartupScreen
	{
		// LoadPreInitModules
		{
			FModuleManager::Get()->LoadModule(TEXT("Engine"));
		}
	}

	// CDO 객체를 생성한다
	for (auto It : GetClassMap()) 
	{
		It.second->GetDefaultObject();
	}

	return 0;
}

int32 FEngineLoop::Init(HWND hViewportWnd)
{
	GEngine = NewObject<UEngine>(nullptr);
	GEngine->Init(hViewportWnd);
	return 0;
}

void FEngineLoop::Tick()
{
	FTaskManager::Get()->Tick();

	GEngine->UpdateTimeAndHandleMaxTickRate();
	GEngine->Tick(FApp::GetDeltaTime());
}

void FEngineLoop::Exit()
{
	GEngine->PreExit();

	FConfigCacheIni::Get(true);
	FTaskManager::Get(true);

	GEngine = nullptr;

	// Unregister Class
	{
		for (auto It : GetClassMap())
		{
			It.second->~UClass();
			GetObjectArray().Free(typeid(UClass), It.second);
			It.second = nullptr;
		}
		GetClassMap().clear();
	}

	{
		GetObjectArray().Destroy();
	}

	FLogger::Get(true);
}

void FEngineLoop::WndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (GEngine)
	{
		GEngine->WndProc(message, wParam, lParam, pResult);
	}
}