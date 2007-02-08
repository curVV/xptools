#include "WED_Document.h"
#include "WED_Progress.h"
#include "XESIO.h"
#include "AptIO.h"
#include "MapAlgs.h"
#include "WED_Messages.h"
#include "WED_DocumentWindow.h"
#include "WED_Thing.h"
#include "WED_PropertyTable.h"

#include "GUI_TextTable.h"
#include "GUI_ScrollerPane.h"
#include "GUI_Splitter.h"
#include "GUI_Table.h"
#include "GUI_Packer.h"

#include "WED_LayerGroup.h"
//#include "WED_ObjectLayers.h"
#include "WED_LayerTable.h"

#include "WED_MapPane.h"


WED_DocumentWindow::WED_DocumentWindow(
	 		const char * 	inTitle, 
	 		int 			inBounds[4],
	 		GUI_Commander * inCommander,
	 		WED_Document *	inDocument) :
	GUI_Window(inTitle, inBounds, inCommander),
	mDocument(inDocument)
{
	
	GUI_Window::SetDescriptor(mDocument->GetFilePath());

	mDocument->AddListener(this);
	
//	mObjects = new WED_ObjectLayers(mDocument->GetObjectRoot());
//	mObjectGroup = new WED_LayerGroup(
//							wed_Layer_Hide | wed_Layer_Rename | wed_Layer_Reorder,
//							wed_Flag_Visible | wed_Flag_Children,
//							"Objects",
//							mObjects);
				
	static const char * titles[] = { "name", "locked", "hidden", 0 };
	static int widths[] = { 100, 50, 50 };
	WED_Thing * root = SAFE_CAST(WED_Thing,mDocument->GetArchive()->Fetch(0));
	mTestTable = new WED_PropertyTable(root,titles, widths);
	mTestTableHeader = new WED_PropertyTableHeader(titles, widths);

//	mLayerTable = new WED_LayerTable;
//	mLayerTable->SetLayers(mObjectGroup);
//	mLayerTableGeometry = new WED_LayerTableGeometry;
//	mLayerTableGeometry->SetLayers(mObjectGroup);
	
	int		splitter_b[4];	
	GUI_Splitter * main_splitter = new GUI_Splitter(gui_Split_Horizontal);
	main_splitter->SetParent(this);
	main_splitter->Show();
	GUI_Pane::GetBounds(splitter_b);
	main_splitter->SetBounds(splitter_b);
	main_splitter->SetSticky(1,1,1,1);
		
	GUI_ScrollerPane * map_scroller = new GUI_ScrollerPane(1,1);
	map_scroller->SetParent(main_splitter);
	map_scroller->Show();
	map_scroller->SetBounds(splitter_b);
	map_scroller->SetSticky(1,1,1,1);
	
	WED_MapPane * map = new WED_MapPane();
	map->SetParent(map_scroller);
	map->Show();
	map_scroller->PositionInContentArea(map);
	map_scroller->SetContent(map);
	
	double	lb[4];
	mDocument->GetBounds(lb);
	map->SetMapVisibleBounds(lb[0], lb[1], lb[2], lb[3]);
	map->SetMapLogicalBounds(lb[0], lb[1], lb[2], lb[3]);


	GUI_Packer * table_packer = new GUI_Packer;
	table_packer->SetParent(main_splitter);
	table_packer->Show();
	table_packer->SetBounds(splitter_b);
	table_packer->SetSticky(0,1,1,1);



		
	GUI_ScrollerPane * layer_scroller = new GUI_ScrollerPane(1,1);
	layer_scroller->SetParent(table_packer);
	layer_scroller->Show();
	layer_scroller->SetBounds(splitter_b);
	layer_scroller->SetSticky(1,1,1,1);
	
	GUI_TextTable * text_table = new GUI_TextTable;
	text_table->SetProvider(mTestTable);
	
	GUI_Table *	layer_table = new GUI_Table;
	layer_table->SetGeometry(mTestTable->GetGeometry());
	layer_table->SetContent(text_table);
	layer_table->SetParent(layer_scroller);
	layer_table->Show();
	layer_scroller->PositionInContentArea(layer_table);
	layer_scroller->SetContent(layer_table);
	


	GUI_TextTableHeader * text_header = new GUI_TextTableHeader;
	text_header->SetProvider(mTestTableHeader);
	text_header->SetGeometry(mTestTable->GetGeometry());
	
	GUI_Header * layer_table_header = new GUI_Header;
	splitter_b[1] = 0;
	splitter_b[3] = 20;
	layer_table_header->SetBounds(splitter_b);	
	layer_table_header->SetGeometry(mTestTable->GetGeometry());
	layer_table_header->SetHeader(text_header);
	layer_table_header->SetParent(table_packer);
	layer_table_header->Show();
	layer_table_header->SetSticky(1,0,1,1);

	layer_table_header->SetTable(layer_table);
	
	
	main_splitter->AlignContents();
	
	table_packer->PackPane(layer_table_header, gui_Pack_Top);
	table_packer->PackPane(layer_scroller, gui_Pack_Center);
	map->ZoomShowAll();
}

WED_DocumentWindow::~WED_DocumentWindow()
{
//	delete mObjects;
//	delete mObjectGroup;	
//	delete mLayerTable;
//	delete mLayerTableGeometry;
	delete mTestTable;
	delete mTestTableHeader;
}

int	WED_DocumentWindow::KeyPress(char inKey, int inVK, GUI_KeyFlags inFlags)
{
	return 0;
}

int	WED_DocumentWindow::HandleCommand(int command)
{
	return 0;
}

int	WED_DocumentWindow::CanHandleCommand(int command, string& ioName, int& ioCheck)
{
	return 0;
}

void	WED_DocumentWindow::ReceiveMessage(
				GUI_Broadcaster *		inSrc,
				int						inMsg,
				int						inParam)
{
}

bool	WED_DocumentWindow::Closed(void)
{
	delete mDocument;
	return false;
}

