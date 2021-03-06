#include "stdafx.h"
#include "wx/wxprec.h"
#include "MainWindow.h"
#include "sampling/debugclient/ComSampler.h"
#include "sampling/ProcessSample.h"
#include "system/Process.h"
#include <memory>
#include "sampling/SamplerOutputQueue.h"
#include "sampling/SamplingTask.h"
#include "VisualizerPane.h"
#include "visualizers/FlowerVisualizer.h"
#include "sampling/SamplingContext.h"
#include "visualizers/OrbVisualizer.h"
#include "ProcessSelectDialog.h"


enum
{
	MenuIdAttachToProcess = 1
};

wxBEGIN_EVENT_TABLE(PerfFlow::MainWindow, wxFrame)
	EVT_MENU(MenuIdAttachToProcess, PerfFlow::MainWindow::onAttachToProcess)
	EVT_MENU(wxID_EXIT, PerfFlow::MainWindow::onExit)
wxEND_EVENT_TABLE()



PerfFlow::MainWindow::MainWindow(const wxString& title, const wxPoint& position, const wxSize& size) : 
	wxFrame(nullptr, wxID_ANY, title, position, size),
	_samplerOutput(std::make_shared<SamplerOutputQueue>(1000))
{
	auto menuFile = new wxMenu;
	menuFile->Append(MenuIdAttachToProcess, "&Attach To Process...\tCtrl-O");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	SetMenuBar(menuBar);


	_visualizerPane = new VisualizerPane(this, _samplerOutput);

	_auiManager.SetManagedWindow(this);

	_symbolListControl = new SymbolListControl(this);

	_auiManager.AddPane(_symbolListControl, wxRIGHT, wxT("Symbols"));
	_auiManager.AddPane(_visualizerPane, wxCENTER);

	_auiManager.Update();
}


PerfFlow::MainWindow::~MainWindow()
{
	_auiManager.UnInit();
}


void PerfFlow::MainWindow::onAttachToProcess(wxCommandEvent& event)
{/*
	auto dialog = new ProcessSelectDialog(this, "Attach To Process...");
	dialog->Show(true);*/
	
	auto processList = Process::CreateProcessList();

	Process process;

	for (auto& processEntry : processList)
	{
		if (processEntry.name() == L"notepad.exe")
			process = processEntry;
	}

	auto context = std::make_shared<SamplingContext>(process);
	_symbolListControl->setContext(context);

	auto sampler = std::make_unique<ComSampler>(context);
	_samplingTask = std::make_unique<SamplingTask>(std::move(sampler), _samplerOutput);
	_samplingTask->begin();

	_visualizerPane->setVisualizer(std::make_unique<OrbVisualizer>(context, _symbolListControl));
}


void PerfFlow::MainWindow::onExit(wxCommandEvent& event)
{

	Close(true);
}
