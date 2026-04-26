#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include <Rolky/HostInstance.hpp>

int main()
{
#ifdef ROLKY_TESTING_DEBUG
	constexpr std::wstring ConfigName = L"Debug";
#else
	constexpr std::wstring ConfigName = L"Release";
#endif

	auto rolkyDir = std::filesystem::current_path().parent_path() / "Build" / ConfigName;
	Rolky::HostSettings settings = {
		.RolkyDirectory = rolkyDir.c_str()
	};
	Rolky::HostInstance hostInstance;
	hostInstance.Initialize(settings);
	return 0;
}