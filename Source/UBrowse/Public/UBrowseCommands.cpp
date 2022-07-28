// Some copyright should be here...

#include "UBrowseCommands.h"

#define LOCTEXT_NAMESPACE "FUBrowseModule"

void FUBrowseCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "UBrowse", "Bring up UBrowse window", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(BrowseUObject, "Show In UBrowse", "Show Details of object in UBrowse", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
