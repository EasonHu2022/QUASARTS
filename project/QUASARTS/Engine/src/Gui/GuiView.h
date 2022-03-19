#pragma once
namespace Engine
{
	class QS_API GuiView
	{
	public:
		GuiView() {};
		~GuiView() {};
	public:
		/*
			on gui update
		*/
		virtual void on_gui() = 0;

		/*
			on view added
		*/
		virtual  void on_add() = 0;

		/*
			on view removed
		*/
		virtual void on_remove() = 0;

	private:

	};

}
