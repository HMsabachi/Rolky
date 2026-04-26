#pragma once

#include "Core.hpp"

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

	struct InternalCallInfo
	{
		const CharType* MethodName;
		void* NativeFuncPtr;
	};

	class HostInstance
	{
	public:
		void Initialize(HostSettings InSettings);

		void AddInternalCall(const CharType* InMethodName, void* InFunctionPtr);

	private:
		void LoadFunctions() const;
		void InitializeRolkyManaged() const;

	private:
		HostSettings m_Settings;
		bool m_Initialized = false;

		std::vector<InternalCallInfo> m_InternalCalls;
	};

}