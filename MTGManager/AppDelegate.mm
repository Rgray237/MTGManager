//
//  AppDelegate.m
//  MTGManager
//
//  Created by Robert Gray on 1/25/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#import "AppDelegate.h"
#import "MyTableView.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (weak) IBOutlet NSImageView *imageView;
@property (weak) IBOutlet NSImageView *imageView2;

@property (weak) IBOutlet NSComboBox *  boxCardName;
@property (weak) IBOutlet NSButton *btnAddToDeck;
@property (weak) IBOutlet NSButton *btnRemoveFromDeck;
@property (weak) IBOutlet NSButton *btnNewDeck;
@property (weak) IBOutlet NSButton *btnDeleteDeck;

@property (weak) IBOutlet NSButton * btnSearchForCard;
@property (weak) IBOutlet NSButton * btnExportCollection;
@property (weak) IBOutlet NSTableView *tblCollection;
@property (weak) IBOutlet NSTableView *tblDecksInCollection;
@property (weak) IBOutlet NSTableView *tblCardsInDeck;


@property (weak) IBOutlet NSTableColumn *col1;
@property (weak) IBOutlet NSTableColumn *col2;
@property (weak) IBOutlet NSTableColumn *colCardsInDeckNames;
@property (weak) IBOutlet NSTableColumn *colCardsInDeckQty;

@property (weak) IBOutlet NSTableColumn *colDeckNames;
@property (weak) IBOutlet NSTextField *lblCurCardName;
@property (weak) IBOutlet NSTextField * lblGrn;
@property (weak) IBOutlet NSTextField * lblWit;
@property (weak) IBOutlet NSTextField * lblRed;
@property (weak) IBOutlet NSTextField * lblBlu;
@property (weak) IBOutlet NSTextField * lblBlk;
@property (weak) IBOutlet NSTextField *lblErrorOut;
@property (weak) IBOutlet NSTextField *lblTotalNumCards;
@property (weak) IBOutlet NSTextField *lblNumUniqueCards;
@property (weak) IBOutlet NSImageView *imgAllManaIcons;
@property (weak) IBOutlet NSTabView * tabs;

@property (weak) IBOutlet NSButton *cbCreatures;
@property (weak) IBOutlet NSButton *cbInstant;
@property (weak) IBOutlet NSButton *cbSorcery;
@property (weak) IBOutlet NSButton *cbLand;
@property (weak) IBOutlet NSButton *cbBlue;
@property (weak) IBOutlet NSButton *cbWhite;
@property (weak) IBOutlet NSButton *cbBlack;
@property (weak) IBOutlet NSButton *cbRed;
@property (weak) IBOutlet NSButton *cbGreen;
@property (weak) IBOutlet NSButton *cbColorless;
@property (weak) IBOutlet NSButton *cbGold;

@property (weak) IBOutlet NSBox *boxHistogram;
@property (strong,nonatomic) NSView *histView;

@end

@implementation AppDelegate

#include "MTGCardsInExistence.hpp"
#include "CollectionManager.hpp"
#include "Card.hpp"
#include <fstream>
//#include "json.hpp"

Card * curCard;
cardsInExistence * allCardsInExistence;
CollectionManager * collectionManager;
NSImage * curCardImage;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    allCardsInExistence = new cardsInExistence();
    collectionManager = new CollectionManager("MTG_Data/myCollection.csv");

    //allCardsInExistence->writeJSONFileToCSVAllCards();

    [self setupTableConnections];
    [self setupComboBoxConnections];
    [self setupImages];
    [self updateStatistics];
}

-(void) setupImages
{
    std::string path = "MTG_Pics/mana.png";
    NSImage * img = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];

    [_imgAllManaIcons setImage:img];
}

-(void) setupTableConnections
{
   [_tblCollection setDataSource:self];
   [_tblDecksInCollection setDataSource:self];
    [_tblCardsInDeck setDataSource:self];
}

-(void) setupComboBoxConnections
{
    [_boxCardName setDataSource:self];
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    //TODO -- not exporting collection properly
    collectionManager->~CollectionManager();
}


- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    if (aTableView==_tblCollection)
        return collectionManager->mainCollection->filteredNumOfUniqueCards();
    if(aTableView==_tblDecksInCollection)
        return collectionManager->numDecksInCollection();
    if(aTableView==_tblCardsInDeck)
    {
        return collectionManager->numCardsInSelectedDeck();
    }
    else
        return 0;
}

-(NSInteger) numberOfItemsInComboBox:(NSComboBox *)comboBox
{
    return [_boxCardName numberOfVisibleItems];
}

- (IBAction)nameEntered:(id)sender {
    [self searchForCard];
}

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index {
    std::string instr = [self nsStrToStdStr:[_boxCardName stringValue]];
    std::vector<std::string> top10 = allCardsInExistence->findTop10Matches(instr);
    NSString * cellValue;

    if (index>allCardsInExistence->getSizeOfTop10())
    {
        cellValue=@"no";
        return cellValue;
    }
    std::string str = top10[index];
    
    if (aComboBox == _boxCardName) {
        cellValue = [NSString stringWithFormat:@"%s",str.c_str()];
    }
    
    return cellValue;
}


-(void) updateTable:(NSTableView *)table
{
        [table reloadData];
}

- (id) tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    NSString* cellValue;

    if (aTableView == _tblCollection)
    {
    std::pair<std::string,int> pr = collectionManager->mainCollection->filteredPairAtIndex((int)rowIndex);
    if (aTableColumn==_col1)
        cellValue = [NSString stringWithFormat:@"%s",pr.first.c_str()];
    else if (aTableColumn == _col2)
        cellValue = [NSString stringWithFormat:@"%d",pr.second];
         
    }
    
    else if (aTableView == _tblDecksInCollection)
    {
            cellValue = [NSString stringWithFormat:@"%s",collectionManager->deckNameAtIndex((int)rowIndex).c_str()];
    }
    else if (aTableView == _tblCardsInDeck)
    {
         mtgCollection *  curClc = collectionManager->getSelectedCollection();
        std::pair<std::string,int> pr = curClc->pairAtIndex((int)rowIndex);
        if (aTableColumn==_colCardsInDeckNames)
            cellValue = [NSString stringWithFormat:@"%s",pr.first.c_str()];
        else if (aTableColumn == _colCardsInDeckQty)
            cellValue = [NSString stringWithFormat:@"%d",pr.second];
    }
    
    return cellValue;
}


-(std::string) nsStrToStdStr:(NSString *)inStr
{
    return std::string([inStr UTF8String]);
}

-(void) updateStatistics
{
    [_lblGrn setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->getGreenCount()]];
    [_lblBlk setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->getBlackCount()]];
    [_lblBlu setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->getBlueCount()]];
    [_lblRed setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->getRedCount()]];
    [_lblWit setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->getWhiteCount()]];
    [_lblNumUniqueCards setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->numOfUniqueCards()]];
    [_lblTotalNumCards setStringValue:[NSString stringWithFormat:@"%d",collectionManager->mainCollection->numOfCards()]];
    [self drawHistogramOfSelectedDeck];
}

- (IBAction)addCardToCollection:(id)sender {
    
    collectionManager->mainCollection->addCardToCollection(curCard);
    [self updateTable:_tblCollection];
    [self updateStatistics];
}

-(NSString * ) nsStrFromStdStr:(std::string)str
{
    NSString * nsstr;
    nsstr = [NSString stringWithFormat:@"%s",str.c_str()];
    return nsstr;
}

-(void) displayMessageWithText:(std::string)str
{
    NSString * nsstr = [self nsStrFromStdStr:str];
    [_lblErrorOut setStringValue:nsstr];
}

- (IBAction)removeCardFromCollection:(id)sender {
    if (collectionManager->canRemoveCardFromMainCollection(curCard))
        collectionManager->mainCollection->removeCardFromCollection(curCard);
    else
    {
        [self displayMessageWithText:collectionManager->getExplanationString()];
    }
    [self updateTable:_tblCollection];
    [self updateStatistics];
}

- (IBAction)addCardToDeck:(id)sender {

    if (collectionManager->canAddThisCardToSelectedDeck(curCard))
    {
        
        collectionManager->addCardToSelectedDeck(curCard);
        [self updateTable:_tblCardsInDeck];
        [self updateStatistics];
    
    }
    else
    {
        [self displayMessageWithText:collectionManager->getExplanationString()];
    }
}
- (IBAction)removeCardFromDeck:(id)sender {
    if (collectionManager->canRemoveThisCardFromSelectedDeck(curCard))
    {
        collectionManager->removeCardFromSelectedDeck(curCard);
        [self updateTable:_tblCardsInDeck];
        [self updateStatistics];
    }
}

-(void) setImageSourceFromPath:(std::string)path
{
    
        curCardImage = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];
        [_imageView setImage:curCardImage];
        [_imageView2 setImage:curCardImage];
    
}

-(void) selectCardAndDisplayImage:(std::string)name set:(std::string)set
{
    if (curCard)
        delete curCard;

    curCard = new Card(name);
    
    [_lblCurCardName setStringValue:[self nsStrFromStdStr:curCard->getName()]];
    
    std::string path;
    
    if(curCard->findImageOfCard())
        path = curCard->pathToImage();
    else
        path = collectionManager->getPathToCardBackImage();
    
    [self setImageSourceFromPath:path];

}

- (IBAction)columnDoubleClicked:(id)sender {
    NSInteger selectedRow = [_tblDecksInCollection selectedRow];
    [_tblDecksInCollection editColumn:0 row:selectedRow withEvent:NULL select:true];
    std::cout<<"hello"<<std::endl;

}



- (IBAction)columnChangeSelected:(id)sender
{
    NSInteger selectedRow = [_tblCollection selectedRow];
    if (selectedRow >=0)
    {
    std::string name = collectionManager->mainCollection->filteredNameAtIndex((int)selectedRow);
    
    if (allCardsInExistence->doesCardExist(name))
    {
        [self selectCardAndDisplayImage:name set:allCardsInExistence->setOfCard(name)];
    }
    }
    
}


- (IBAction)deckCardSelected:(id)sender {
    NSInteger cardSelected = [_tblCardsInDeck selectedRow];
    if (cardSelected>=0)
    {
        std::string selectedName = collectionManager->cardNameInSelectedDeckAtIndex((int)cardSelected);
        if (allCardsInExistence->doesCardExist(selectedName))
        {
        [self selectCardAndDisplayImage:selectedName set:allCardsInExistence->setOfCard(selectedName)];
        }
        
    }
}


- (IBAction)deckRowSelected:(id)sender {
    NSInteger deckSelected = [_tblDecksInCollection selectedRow];
    if (deckSelected>=0)
    {
    collectionManager->setSelectedCollection((int)deckSelected);
    
    [self updateTable:_tblCardsInDeck];
    [self updateStatistics];
    }
}





-(void) searchForCard
{
    std::string name,set;
    name = [[_boxCardName stringValue] UTF8String];
    
    
    if (allCardsInExistence->doesCardExist(name))
    {
        set = allCardsInExistence->setOfCard(name);
        [self selectCardAndDisplayImage:name set:set];
    }
}

- (IBAction)searchForCard:(id)sender {
    [self searchForCard];
}

- (IBAction)exportCollection:(id)sender {
    collectionManager->exportCollection();
}

- (IBAction)addDeck:(id)sender {
    collectionManager->addDeck("default");

    [self updateTable:_tblDecksInCollection];
}

- (IBAction)deleteDeck:(id)sender {
    collectionManager->deleteSelectedDeck();
    [self updateTable:_tblDecksInCollection];
    [self updateTable:_tblCardsInDeck];
}

-(void) clearHistogram
{
    [_histView removeFromSuperview];
}

-(void) drawHistogramOfSelectedDeck
{
    [self clearHistogram];
    _histView = [[NSView alloc] initWithFrame:CGRectMake(0, 0, _boxHistogram.frame.size.width, _boxHistogram.frame.size.height)];
    
    [_boxHistogram addSubview:_histView];

    std::string path = "MTG_Pics/bar.png";
    NSImage * img =[[NSImage alloc] initWithContentsOfFile:[NSString stringWithUTF8String:path.c_str()]];

    
    if (collectionManager->numCardsInSelectedDeck()!=0)
    {

    for(int i=0;i<11;i++)
    {
        
        float heightPercentage = collectionManager->ratioOfHistogramValueWithPeakOfHistogram(i);
        float xaxisYPos = _histView.frame.size.height/10;
        NSImageView * imgView = [[NSImageView alloc] initWithFrame:CGRectMake(i*_histView.frame.size.width/11, xaxisYPos, _histView.frame.size.width/11, heightPercentage*_histView.frame.size.height)];
        NSTextField * xaxisLabel = [[NSTextField alloc] initWithFrame:CGRectMake(i*_histView.frame.size.width/11, 0, _histView.frame.size.width/11, 20)];
        if (i==10)
            xaxisLabel.stringValue = @"10+";
       else
           xaxisLabel.stringValue = [self nsStrFromStdStr:std::to_string(i)];
        xaxisLabel.font = [NSFont fontWithName:@"Times-Roman" size:12];
        xaxisLabel.backgroundColor = [NSColor clearColor];
        [xaxisLabel setBordered:false];
        [xaxisLabel setBezeled:false];
        [xaxisLabel setEditable:false];
        [xaxisLabel setDrawsBackground:true];
        [xaxisLabel setAlignment:NSTextAlignmentCenter];
        
        
        NSTextField * txt = [[NSTextField alloc] initWithFrame:CGRectMake(i*_histView.frame.size.width/11, imgView.frame.size.height/2+xaxisYPos, _histView.frame.size.width/11, 20)];
        txt.stringValue = [self nsStrFromStdStr:std::to_string(collectionManager->histogramValueOfSelectedDeckAtIndex(i))];
        txt.backgroundColor = [NSColor clearColor];
        txt.font = [NSFont fontWithName:@"Times-Roman" size:12];
        [txt setEditable:false];
        [txt setBezeled:false];
        [txt setBordered:false];
        [txt setDrawsBackground:true];
        [txt setAlignment:NSTextAlignmentCenter];
        
        imgView.imageScaling = NSImageScaleAxesIndependently;
        imgView.image = img;
        [_histView addSubview:imgView];
        [_histView addSubview:txt];
        [_histView addSubview:xaxisLabel];
    }
    }
}

- (IBAction)checkBoxSelected:(NSButton *)sender {

    bool on = sender.state == NSOnState ? true : false;
    std::cout<<"on? "<<on<<std::endl;
    
    if (sender==_cbCreatures)
        std::cout<<"critters"<<std::endl;
    if (sender==_cbRed)
        collectionManager->mainCollection->setColorFilter(filtRed,on);
    if (sender==_cbBlue)
        collectionManager->mainCollection->setColorFilter(filtBlue,on);
    if (sender==_cbBlack)
        collectionManager->mainCollection->setColorFilter(filtBlack,on);
    if (sender==_cbWhite)
        collectionManager->mainCollection->setColorFilter(filtWhite,on);
    if (sender==_cbGreen)
        collectionManager->mainCollection->setColorFilter(filtGreen,on);
    if (sender==_cbColorless)
        collectionManager->mainCollection->setColorFilter(filtColorless, on);
    if (sender==_cbGold)
        collectionManager->mainCollection->setColorFilter(filtMulti, on);
    if (sender==_cbCreatures)
        collectionManager->mainCollection->setTypeFilter(filtCreatures, on);
    if (sender==_cbSorcery)
        collectionManager->mainCollection->setTypeFilter(filtSorcery, on);
    if (sender==_cbInstant)
        collectionManager->mainCollection->setTypeFilter(filtInstant, on);
    if (sender==_cbLand)
        collectionManager->mainCollection->setTypeFilter(filtLand, on);
    
    
    [self updateTable:_tblCollection];
}


@end
