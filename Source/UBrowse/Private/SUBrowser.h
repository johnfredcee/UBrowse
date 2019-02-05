#pragma once


class IDetailsView;

namespace EQuerySortMode
{
	enum Type
	{
		ByID,
		ByType,
		ByNumber
	};
}

struct FUBrowserPanel
{

	TArray< TSharedPtr<FBrowserObject> > LiveObjects;
	TSharedPtr<SUBrowsePanel>            BrowsePanel;
};


class SUBrowser :	public SCompoundWidget
{
	friend FBrowserObject;
public:
	SLATE_BEGIN_ARGS(SUBrowser) { }
	SLATE_END_ARGS()

	/**
	* Construct this widget
	*
	* @param InArgs The declaration data for this widget.
	*/
	void Construct(const FArguments& InArgs);

	void RefreshList();


private:

	DECLARE_DELEGATE_OneParam(FOnNewObjectView, FBrowserObject*);

	FOnNewObjectView OnNewObjectView;

	void OnObjectListSelectionChanged(TSharedPtr<FBrowserObject> InItem, ESelectInfo::Type SelectInfo);

	void OnNewHostTextCommited(const FText& InText, ETextCommit::Type InCommitType);

	TSharedRef<ITableRow> OnGenerateObjectListRow(TSharedPtr<FBrowserObject> ObjectPtr, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<ITableRow> HandlePropertyGenerateRow(TSharedPtr<FBrowserObject> ObjectPtr, const TSharedRef<STableViewBase>& OwnerTable);

	FText GetFilterClassText() const;

	FReply OnClassSelectionClicked();

	FReply OnCollectGarbage();

	void AddBoolFilter(FMenuBuilder& MenuBuilder, FText Text, FText ToolTip, bool* BoolOption);

	TSharedRef<SWidget> MakeFilterMenu();

	void OnNodeDoubleClicked(class UEdGraphNode* Node);

	void OnGetChildrenForTree(TWeakObjectPtr<UObject> InClass, TArray< TWeakObjectPtr<UObject> >& OutChildren);

	TSharedRef<ITableRow> OnGenerateHistoryRow(TSharedPtr<FBrowserObject> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	void  OnHistorySelectionChanged(TSharedPtr<FBrowserObject> InItem, ESelectInfo::Type /*SelectInfo*/);

	void OnSortByChanged(const EColumnSortPriority::Type SortPriority, const FName& ColumnName, const EColumnSortMode::Type NewSortMode);

	EColumnSortMode::Type GetSortMode() const;

	TSharedPtr<SWidget> GetTreeContextMenu();

	void AddObjectToHistory(TSharedPtr<FBrowserObject> Item);

	void PopulateHistoryList();
	
	FUBrowserPanel& GetCurrentBrowserPanel();

	const TArray< TSharedPtr<FBrowserObject> >& GetLiveObjects();

	const TArray< TSharedPtr< FBrowserObject > >& GetCurrentHistoryList();

	// Filters
	FText FilterText;
	FString FilterString;
	UClass* FilterClass;
	bool bShouldIncludeClassDefaultObjects;	
	bool bShouldIncludeDefaultSubObjects;
	bool bShouldIncludeArchetypeObjects;
	bool bOnlyListRootObjects;
	bool bOnlyListGCObjects;
	bool bIncludeTransient;

	// Holds the widget switcher.
	TSharedPtr<SWidgetSwitcher> UBrowseSwitcher;

	/* List of objects of selectable class - used to zoom in on what's intersting */
	TSharedPtr< SListView< TSharedPtr<FBrowserObject> > > ObjectListView;

	/* List of objects we have browsed */
	TSharedPtr< SListView< TSharedPtr< FBrowserObject > > >  ObjectHistoryView;

	/* Customized detail view we use for examining properties */
	TSharedPtr<IDetailsView> PropertyView;

	/* Graph panels for visualising structure (more than one of them - switched by history browsing) */
	TArray< TSharedPtr<FUBrowserPanel> > BrowserPanels;

	/* History of objects browsed */
	TArray< TSharedPtr< FBrowserObject >  > History;

	/** Current way we are sorting queries */
	EQuerySortMode::Type			SortBy;

	/** Current way we are setting ID sort direction */
	EColumnSortMode::Type			SortDirection;

};
	

