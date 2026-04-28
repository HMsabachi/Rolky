#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include <Rolky/HostInstance.hpp>


void Dummy()
{
	std::cout << "Dummy!" << std::endl;
}

int main()
{
#ifdef ROLKY_TESTING_DEBUG
	constexpr std::wstring ConfigName = L"Debug";
#else
	constexpr std::wstring ConfigName = L"Release";
#endif

	auto rolkyDir = std::filesystem::current_path().parent_path() / "Build" / ConfigName;
	Rolky::HostSettings settings = 
	{
		.RolkyDirectory = rolkyDir.c_str()
	};
	Rolky::HostInstance hostInstance;
	hostInstance.Initialize(settings);

	Rolky::AssemblyHandle testingHandle;
	hostInstance.LoadAssembly(ROLKY_STR("E:/PrismEngine/Rolky/Build/Debug/Testing.Managed.dll"), testingHandle);

	hostInstance.AddInternalCall(ROLKY_STR("Rolky.ManagedHost+Dummy, Rolky.Managed"), &Dummy);
	hostInstance.AddInternalCall(ROLKY_STR("Rolky.ManagedHost+Dummy, Rolky.Managed"), &Dummy);
	hostInstance.UploadInternalCalls();

	Rolky::ObjectHandle objectHandle = hostInstance.CreateInstance(ROLKY_STR("Testing.MyTestObject, Testing.Managed"));
	// hostInstance.CallMethod(objectHandle, "MyInstanceMethod", 5.0f, 10.0f, myOtherObjectHandle);
	hostInstance.DestroyInstance(objectHandle);
	return 0;
}