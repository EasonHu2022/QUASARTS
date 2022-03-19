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

	void on_update();

	void swap_buffer();

	unsigned int get_width() const { return m_Data.width; }
	unsigned int get_height() const { return m_Data.height; }
	
	//later change to unique pointer
	static Window* create(const WindowProps& props = WindowProps());
	// Window attributes


	GLFWwindow* get_native_window() const { return m_Window; }
public:
	void init(const WindowProps& props);
	void shutdown();
private:
	GLFWwindow* m_Window;


	struct WindowData
	{
		std::string title;
		unsigned int width, height;
		bool vSync;
	};

	WindowData m_Data;
};


