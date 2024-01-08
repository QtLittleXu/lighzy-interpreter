#include "program/initialization.h"
#include <unistd.h>
#include <vector>

namespace li::program 
{


#ifdef __WIN32

filesystem::path executable_path()
{
	// To do
	return "";
}

#elif __linux__

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