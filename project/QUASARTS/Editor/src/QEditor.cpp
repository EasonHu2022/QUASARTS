#include "QuasartsEngine.h"
#include "GuiViews/MenuBarView.h"
#include "GuiViews/FileInputView.h"

/*
	Entry point
*/
#include "Core/EntryPoint.h"

class QEditor : public Engine::Application
{
public:
	QEditor()
	{
		QDEBUG("create");
		//push views into app
		auto menuBar = new MenuBarView();
		auto fileInput = new FileInputView();
		Engine::GuiWrapper::Instance->add_gui_view(menuBar);
		Engine::GuiWrapper::Instance->add_gui_view(fileInput);
	};
	~QEditor() {};

private:

};

Engine::Application* Engine::create_application()
{
	return new QEditor();
}