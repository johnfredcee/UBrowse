// Some copyright should be here...

#include "UBrowseCommands.h"
#include "UBrowsePrivatePCH.h"

#define LOCTEXT_NAMESPACE "FUBrowseModule"

void FUBrowseCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UBrowse", "Bring up UBrowse window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
