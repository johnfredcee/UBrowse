#pragma once

#include "ClassIconFinder.h"

#define LOCTEXT_NAMESPACE "SUBrowserClassItem"

class SUBrowserClassItem : public SMultiColumnTableRow< TWeakObjectPtr< UObject > > 
{

public:
    SLATE_BEGIN_ARGS( SUBrowserClassItem ) 
        : _ClassToView()
        {}
        SLATE_ARGUMENT( TWeakObjectPtr< UObject >, ClassToView )
    SLATE_END_ARGS()

	BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
    TSharedRef<SWidget> GenerateWidgetForColumn( const FName& ColumName ) override 
	{
        FSlateFontInfo ClassBrowseFont = FCoreStyle::Get().GetFontStyle(TEXT("NormalFont"));

        if ( ColumName == TEXT("Name") ) 
        {
			TSharedRef<SWidget> ColumnContent = SNew(SBox)
								.Padding(FMargin(4.0f, 4.0f))
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
									.Font(ClassBrowseFont)
									.Text(this, &SUBrowserClassItem::OnReadText)
								];

            return    SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.AutoWidth()
							.VAlign(VAlign_Center)
							[
								SNew(SExpanderArrow, SharedThis( this ))
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ColumnContent
							];
		}
        else if ( ColumName == TEXT("Count") )
        {
			TSharedRef<SWidget> ColumnContent = SNew(SBox)
				.Padding(FMargin(4.0f, 4.0f))
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(ClassBrowseFont)
					.Text(this, &SUBrowserClassItem::OnCalcCount)
				];
			return ColumnContent;
		}
		else
		{
			return
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("UnsupprtedColumnText", "Unsupported Column: {0}"), FText::FromName(ColumName)));
		}
    }
	END_SLATE_FUNCTION_BUILD_OPTIMIZATION

    void Construct( const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView )
	{
		ClassToView = InArgs._ClassToView;

		SMultiColumnTableRow< TWeakObjectPtr<UObject> >::Construct(FSuperRowType::FArguments(), InOwnerTableView);
    }

private:
    FText OnReadText() const 
    {
        return FText::FromString(ClassToView->GetName());
    }

	FText OnCalcCount() const
	{
		uint32_t InstanceCount = 0;
		for (FThreadSafeObjectIterator Itr; Itr; ++Itr) {
			if (Itr->IsA(Cast<UClass>(ClassToView.Get())))
				InstanceCount++;
		}
		return FText::FromString(FString::FromInt(InstanceCount));
	}

   	/** A pointer to the data item that we visualize/edit */
	TWeakObjectPtr< UObject > ClassToView;

};

#undef LOCTEXT_NAMESPACE