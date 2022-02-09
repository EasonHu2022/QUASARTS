#pragma once
#include "IModule.h"
class  MemoryModule : public IModule
{
private:
	static MemoryModule* instance;
public:
	static MemoryModule* Instance();
public:
	/// <summary>
	/// init the memoryModule
	/// </summary>
	void init() override;


	/// <summary>
	/// start the memoryModule
	/// </summary>
	/// <returns></returns>
	int start() override;

	/// <summary>
	/// update
	/// </summary>
	void update() override;

	/// <summary>
	/// stop the Memory module
	/// </summary>
	/// <returns></returns>
	int stop() override;

	/// <summary>
	/// release the memory module
	/// </summary>
	void release() override;
};
