#include "stdafx.h"
#include "PerfFlowUIApp.h"


wxIMPLEMENT_APP(PerfFlow::PerfFlowUI);

bool PerfFlow::PerfFlowUI::OnInit()
{
	auto frame = new MainWindow("PerfFlow", wxPoint(50, 50), wxSize(800, 800));
	frame->Show(true);

	return true;
}