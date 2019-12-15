// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Misc/CompilationResult.h"

class FLineNotify4UnrealEdModule : public IModuleInterface
{
private:
	// Handle of registered event
	FDelegateHandle OnCompileFinishedHandle;
	FDelegateHandle OnLightBuildSucceededHandle;
	FDelegateHandle OnLightBuildFailedHandle;

public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Called when compilation is complete
	void OnCompileFinished(const FString& LogDump, ECompilationResult::Type CompilationResult, bool bShowLog) const;

	// Called when light build is successful
	void OnLightBuildSucceeded();

	// Called when a light build fails
	void OnLightBuildFailed();
};
