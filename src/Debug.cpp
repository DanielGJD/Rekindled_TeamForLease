#include "Debug.h"
namespace ForLeaseEngine
{
	Debug::Debug(const std::string& filename)
	{
		file.open(filename, std::ios_base::out);
	}

	bool Debug::IsOpen()
	{
		return file.is_open();
	}

	Debug::~Debug()
	{
		file.close();
	}

	Debug& Debug::operator<<(std::string& string)
	{
		file << string << std::endl;
		return *this;
	}
}