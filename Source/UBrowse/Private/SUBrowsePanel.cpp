#include "UBrowsePrivatePCH.h"
#include "UBrowseGraph.h"
#include "UBrowseSchema.h"
#include "SUBrowsePanel.h"

#define LOCTEXT_NAMESPACE "SUBrowsePanel"

void SUBrowsePanel::Construct(const FArguments& InArgs)
{
	pBrowserGraph = NewObject< UBrowseGraph >( UBrowseGraph::StaticClass() );
	pBrowserGraph->Schema = UBrowseSchema::StaticClass();
	pBrowserGraph->AddToRoot();
	OnNewObjectView.BindSP(this, &SUBrowsePanel::OnNewRootNode);
	// appearance
	// TODO: Name of object at root
	AppearanceInfo.CornerText = LOCTEXT("UBrowseGraphCornerText", "UBROWSE");
	// events
	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnNodeDoubleClicked = InArgs._OnNodeDoubleClicked;
	pGraphEditor = SNew(SGraphEditor)
		.GraphToEdit(pBrowserGraph)
		.IsEditable(false) 
		.TitleBar(SNew(SBorder).HAlign(HAlign_Center)
		[
			SNew(STextBlock).Text(LOCTEXT("UBrowseGraphTitle", "UBrowse Graph"))
		])
		.GraphEvents(GraphEvents)
		.DisplayAsReadOnly(false)
		.Appearance(this, &SUBrowsePanel::GetAppearanceInfo);
		ChildSlot
		[
			pGraphEditor.ToSharedRef()
		];
	pBrowserGraph->RefreshGraph();
}

/*
void SUBrowsePanel::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
}
*/

FGraphAppearanceInfo SUBrowsePanel::GetAppearanceInfo() const
{
	return AppearanceInfo;
}

void SUBrowsePanel::OnNewRootNode(FBrowserObject* InObject)
{
	AppearanceInfo.CornerText = FText::FromString(GetNameSafe(InObject->Object.Get()));
	AppearanceInfo.ReadOnlyText = FText::FromString(GetNameSafe(InObject->Object.Get()));
	pBrowserGraph->RefreshGraph(InObject->Object.Get());
}


#undef LOCTEXT_NAMESPACE