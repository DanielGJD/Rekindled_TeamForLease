#ifndef DEBUG_H
#define DEBUG_H
#include <fstream>
#include <string>

namespace ForLeaseEngine
{
	class Debug
	{
	private:
		std::fstream file;

	public:
		Debug(const std::string& filename = "Debug.txt");
		bool IsOpen();
		Debug& operator<<(std::string& string);
		~Debug();
	};
}

#endif
