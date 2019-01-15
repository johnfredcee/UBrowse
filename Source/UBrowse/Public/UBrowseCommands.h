// Some copyright should be here...

#pragma once

#include "SlateBasics.h"
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

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};