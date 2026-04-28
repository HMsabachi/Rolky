#pragma once

#include "Core.hpp"
#include "Assembly.hpp"

#include <unordered_map>

namespace Rolky {

	using ErrorCallbackFn = void (*)(const CharType* InMessage);

	struct HostSettings
	{
		/// <summary>
		/// The file path to Rolky.runtimeconfig.json (e.g C:\Dev\MyProject\ThirdParty\Rolky)
		/// </summary>
		const CharType* RolkyDirectory;

		ErrorCallbackFn ErrorCallback = nullptr;
	};


	class HostInstance
	{
	public:
		void Initialize(HostSettings InSettings);
		AssemblyLoadStatus LoadAssembly(const CharType* InFilePath, AssemblyHandle& OutHandle);
		void AddInternalCall(const CharType* InMethodName, void* InFunctionPtr);

		void UploadInternalCalls();

	private:
		void LoadHostFXR() const;
		void InitializeRolkyManaged();

		void* LoadRolkyManagedFunctionPtr(const std::filesystem::path& InAssemblyPath, const CharType* InTypeName, const CharType* InMethodName, const CharType* InDelegateType = ROLKY_UNMANAGED_CALLERS_ONLY) const;

		template <typename TFunc>
		TFunc LoadRolkyManagedFunctionPtr(const CharType* InTypeName, const CharType* InMethodName, const CharType* InDelegateType = ROLKY_UNMANAGED_CALLERS_ONLY) const
		{
			return (TFunc)LoadRolkyManagedFunctionPtr(m_RolkyManagedAssemblyPath, InTypeName, InMethodName, InDelegateType);
		}

	public:
		struct InternalCall
		{
			const CharType* Name;
			void* NativeFunctionPtr;
		};

	private:
		HostSettings m_Settings;
		std::filesystem::path m_RolkyManagedAssemblyPath;
		void* m_HostFXRContext = nullptr;
		bool m_Initialized = false;

		std::vector<InternalCall*> m_InternalCalls;

		//std::unordered_map<uint16_t, AssemblyData> m_LoadedAssemblies;
	};

}