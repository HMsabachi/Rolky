#include "HostInstance.hpp"
#include "Verify.hpp"
#include "HostFXRErrorCodes.hpp"

namespace Rolky {

	hostfxr_set_error_writer_fn SetHostFXRErrorWriter = nullptr;
	hostfxr_initialize_for_runtime_config_fn InitHostFXRForRuntimeConfig = nullptr;
	hostfxr_get_runtime_delegate_fn GetRuntimeDelegate = nullptr;
	hostfxr_close_fn CloseHostFXR = nullptr;

	ErrorCallbackFn ErrorCallback = nullptr;

	void DefaultErrorCallback(const CharType* InMessage)
	{
#if ROLKY_WIDE_CHARS
		std::wcout << L"[Rolky.Native]: " << InMessage << std::endl;
#else
		std::cout << L"[Rolky.Native]: " << InMessage << std::endl;
#endif
	}

	void HostInstance::Initialize(HostSettings InSettings)
	{
		if (m_Initialized)
		{
			// TODO: Error here
			return;
		}

		LoadFunctions();

		// Setup settings
		m_Settings = std::move(InSettings);

		if (!m_Settings.ErrorCallback)
			m_Settings.ErrorCallback = DefaultErrorCallback;
		ErrorCallback = m_Settings.ErrorCallback;

		SetHostFXRErrorWriter([](const CharType* InMessage)
		{
			ErrorCallback(InMessage);
		});

		InitializeRolkyManaged();

		m_Initialized = true;
	}

	void HostInstance::AddInternalCall(const CharType* InMethodName, void* InFunctionPtr)
	{
		ROLKY_VERIFY(InFunctionPtr != nullptr);

		auto& icallData = m_InternalCalls.emplace_back();
		icallData.MethodName = InMethodName;
		icallData.NativeFuncPtr = InFunctionPtr;
	}

#ifdef _WIN32
	template <typename TFunc>
	TFunc LoadFunctionPtr(void* InLibraryHandle, const char* InFunctionName)
	{
		auto result = (TFunc)GetProcAddress((HMODULE)InLibraryHandle, InFunctionName);
		ROLKY_VERIFY(result);
		return result;
	}
#else
	template <typename TFunc>
	TFunc LoadFunctionPtr(void* InLibraryHandle, const char* InFunctionName)
	{
		auto result = (TFunc)dlsym(InLibraryHandle, InFunctionName);
		ROLKY_VERIFY(result);
		return result;
	}
#endif

	void HostInstance::LoadFunctions() const
	{
		// Retrieve the file path to the CoreCLR library
		size_t pathBufferSize = 0;
		int status = get_hostfxr_path(nullptr, &pathBufferSize, nullptr);
		ROLKY_VERIFY(status == StatusCode::HostApiBufferTooSmall);
		std::vector<CharType> pathBuffer;
		pathBuffer.resize(pathBufferSize);
		status = get_hostfxr_path(pathBuffer.data(), &pathBufferSize, nullptr);
		ROLKY_VERIFY(status == StatusCode::Success);

		// Load the CoreCLR library
		void* libraryHandle = nullptr;

#ifdef _WIN32
	#ifdef _WCHAR_T_DEFINED
		libraryHandle = LoadLibraryW(pathBuffer.data());
	#else
		libraryHandle = LoadLibraryA(pathBuffer.data());
	#endif
#else
		libraryHandle = dlopen(pathBuffer.data());
#endif

		ROLKY_VERIFY(libraryHandle != nullptr);

		// Load CoreCLR functions
		SetHostFXRErrorWriter = LoadFunctionPtr<hostfxr_set_error_writer_fn>(libraryHandle, "hostfxr_set_error_writer");
		InitHostFXRForRuntimeConfig = LoadFunctionPtr<hostfxr_initialize_for_runtime_config_fn>(libraryHandle, "hostfxr_initialize_for_runtime_config");
		GetRuntimeDelegate = LoadFunctionPtr<hostfxr_get_runtime_delegate_fn>(libraryHandle, "hostfxr_get_runtime_delegate");
		CloseHostFXR = LoadFunctionPtr<hostfxr_close_fn>(libraryHandle, "hostfxr_close");
	}

	void HostInstance::InitializeRolkyManaged() const
	{
		load_assembly_and_get_function_pointer_fn loadRolkyAssemblyFunc = nullptr;

		// Fetch load_assembly_and_get_function_pointer_fn from CoreCLR
		{
			hostfxr_handle context = nullptr;
			auto runtimeConfigPath = std::filesystem::path(m_Settings.RolkyDirectory) / "Rolky.Managed.runtimeconfig.json";
			int status = InitHostFXRForRuntimeConfig(runtimeConfigPath.c_str(), nullptr, &context);
			ROLKY_VERIFY(status == StatusCode::Success && context != nullptr);

			status = GetRuntimeDelegate(context, hdt_load_assembly_and_get_function_pointer, (void**)&loadRolkyAssemblyFunc);
			ROLKY_VERIFY(status == StatusCode::Success && loadRolkyAssemblyFunc != nullptr);

			CloseHostFXR(context);
		}

		auto rolkyAssemblyPath = std::filesystem::path(m_Settings.RolkyDirectory) / "Rolky.Managed.dll";
		component_entry_point_fn rolkyManagedEntryPoint = nullptr;
		int status = loadRolkyAssemblyFunc(rolkyAssemblyPath.c_str(), ROLKY_STR("Rolky.ManagedHost, Rolky.Managed"), ROLKY_STR("Initialize"), nullptr, nullptr, (void**)&rolkyManagedEntryPoint);
		ROLKY_VERIFY(status == StatusCode::Success && rolkyManagedEntryPoint != nullptr);

		struct DummyData
		{
			float x = 10.0f;
			const CharType* Str = ROLKY_STR("Hello from native code!");
		} dummyData;

		rolkyManagedEntryPoint(&dummyData, sizeof(dummyData));
	}

}