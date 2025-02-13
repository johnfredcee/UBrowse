
#include "UBrowseEditorCommands.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "UBrowseEditorCommands"

FUBrowseEditorCommands::FUBrowseEditorCommands() : TCommands<FUBrowseEditorCommands>(
	"UBrowseEditorCommands",
	NSLOCTEXT("Contexts", "UBrowseEditorCommands", "UObject Browsing"),
	NAME_None, 
	FAppStyle::GetAppStyleSetName())
{
}

void FUBrowseEditorCommands::RegisterCommands()
{
    UI_COMMAND(ViewAsset, "Open In Asset Editor...", "Opens the Asset in the asset editor", EUserInterfaceActionType::Button, FInputChord(EModifierKey::Shift | EModifierKey::Alt, EKeys::A));
}
#undef LOCTEXT_NAMESPACE
