// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "test5GameMode.h"
#include "test5Pawn.h"
#include "test5Hud.h"

Atest5GameMode::Atest5GameMode()
{
	DefaultPawnClass = Atest5Pawn::StaticClass();
	HUDClass = Atest5Hud::StaticClass();
}
