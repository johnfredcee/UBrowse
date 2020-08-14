
#pragma once

#include "CoreMinimal.h"
#include "EditorStyleSet.h"
#include "Framework/Commands/Commands.h"

class FUBrowseEditorCommands : public TCommands<FUBrowseEditorCommands>
{  
public:
	FUBrowseEditorCommands();

	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

	// Shows the reference viewer for the selected assets
	TSharedPtr<FUICommandInfo> ViewAsset;

};