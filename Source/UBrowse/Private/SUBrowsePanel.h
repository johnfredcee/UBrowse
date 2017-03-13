#pragma once

#include "Engine.h"
#include "GraphEditor.h"

class SUBrowsePanel : public SCompoundWidget
{
public:
	DECLARE_DELEGATE_OneParam(FOnNewObjectView, FBrowserObject*);

	SLATE_BEGIN_ARGS(SUBrowsePanel) { }
		SLATE_EVENT(FSingleNodeEvent, OnNodeDoubleClicked )
	SLATE_END_ARGS()

	FOnNewObjectView OnNewObjectView;

	FSingleNodeEvent OnNodeDoubleClicked;
	
	/**
	* Construct this widget
	*
	* @param InArgs The declaration data for this widget.
	*/
	void Construct(const FArguments& InArgs);

	/* Called when a new root node is selected */
	void OnNewRootNode(FBrowserObject* object);

	FGraphAppearanceInfo GetAppearanceInfo() const;

private:
	FGraphAppearanceInfo 	 AppearanceInfo;
	TSharedPtr<SGraphEditor> pGraphEditor;
	UBrowseGraph 			*pBrowserGraph;
};