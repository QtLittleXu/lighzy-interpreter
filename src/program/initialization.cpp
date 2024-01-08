#include "program/initialization.h"
#include <vector>

namespace li::program 
{


// Only support executable_path() on Windows and Linux now

#ifdef __WIN32__

#include <windows.h>

filesystem::path executable_path()
{
	std::vector<char> buf(1024, 0);
	std::size_t size = buf.size();
	bool havePath = false;
	bool shouldContinue = true;

	do
	{
		DWORD result = GetModuleFileNameA(nullptr, buf.data(), size);
		DWORD lastError = GetLastError();
		if (result == 0)
		{
			shouldContinue = false;
		}
		else if (result < size)
		{
			havePath = true;
			shouldContinue = false;
		}
		else if (result == size && (lastError == ERROR_INSUFFICIENT_BUFFER || lastError == ERROR_SUCCESS))
		{
			size *= 2;
			buf.resize(size);
		}
		else
		{
			shouldContinue = false;
		}
	} while (shouldContinue);

	if (!havePath)
	{
		return "";
	}

	std::string path = buf.data();
	return path;
}

#elif __linux__

#include <unistd.h>

filesystem::path executable_path()
{
	std::vector<char> buf(1024, 0);
	std::size_t size = buf.size();
	bool havePath = false;
	bool shouldContinue = true;

	do
	{
		ssize_t result = readlink("/proc/self/exe", buf.data(), size);
		if (result < 0)
		{
			shouldContinue = false;
		}
		else if (static_cast<std::size_t>(result) < size)
		{
			havePath = true;
			shouldContinue = false;
			size = result;
		}
		else
		{
			size *= 2;
			buf.resize(size);
			std::fill(std::begin(buf), std::end(buf), 0);
		}
	} while (shouldContinue);

	if (!havePath)
	{
		return "";
	}

	string path(buf.data(), size);
	return path;
}

#endif


}