// Some copyright should be here...

#pragma once

#include "UBrowseStyle.h"

class FUBrowseCommands : public TCommands<FUBrowseCommands>
{
public:

	FUBrowseCommands()
		: TCommands<FUBrowseCommands>(TEXT("UBrowse"), NSLOCTEXT("Contexts", "UBrowse", "UBrowse Plugin"), NAME_None, FUBrowseStyle::GetStyleSetName())
	{
	}


	// TCommands<> interface
	virtual void RegisterCommands() override;
	// End of TCommands<> interface

	// Goes direct to the plugin window
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	// Goes to the plugin window via specific UObject
	TSharedPtr< FUICommandInfo > BrowseUObject;
};