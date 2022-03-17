#pragma once
#include <glfw3.h>
#include <string>

struct WindowProps
{
	std::string Title;
	uint32_t Width;
	uint32_t Height;

	WindowProps(const std::string& title = "Quasarts Editor",
		uint32_t width = 1600,
		uint32_t height = 900)
		: Title(title), Width(width), Height(height)
	{
	}
};


class Window
{
public:
	Window(const WindowProps& props);
	~Window();

	void OnUpdate();

	unsigned int GetWidth() const { return m_Data.Width; }
	unsigned int GetHeight() const { return m_Data.Height; }
	
	//later change to unique pointer
	static Window* Create(const WindowProps& props = WindowProps());
	// Window attributes


	GLFWwindow* GetNativeWindow() const { return m_Window; }
private:
	void Init(const WindowProps& props);
	void Shutdown();
private:
	GLFWwindow* m_Window;


	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;
	};

	WindowData m_Data;
};


