#include "Plugin.h"
#include "RE/Offset.Ext.h"

#include "REL/Module.h"
#include "REL/Pattern.h"
#include "REL/Relocation.h"
#include "stl/stl.h"

#include "RE/Bethesda/BS/BSFixedString.h"
#include "RE/Bethesda/MouseMoveEvent.h"
#include "RE/Bethesda/UserEvents.h"
#include "RE/Offset.h"

#include <ShlObj_core.h>
#include <array>
#include <memory>
#include <utility>

#define PLUGIN_MODE

#include "DKUtil/Config.hpp"
#include "DKUtil/Hook.hpp"
#include "DKUtil/Logger.hpp"

#define DLLEXPORT extern "C" [[maybe_unused]] __declspec(dllexport)

using namespace DKUtil::Alias;

namespace RE
{
	class BSInputDeviceManager;

	namespace PlayerControls
	{
		class LookHandler;
	}
}

bool IsUsingGamepad(RE::BSInputDeviceManager* a_inputDeviceManager)
{
	using func_t = decltype(IsUsingGamepad);
	REL::Relocation<func_t> func{ RE::Offset::BSInputDeviceManager::IsUsingGamepad };
	return func(a_inputDeviceManager);
}

static bool UsingThumbstickLook = false;

bool IsUsingThumbstickLook(RE::BSInputDeviceManager*)
{
	return UsingThumbstickLook;
}

bool IsGamepadCursor(RE::BSInputDeviceManager* a_inputDeviceManager)
{
	return UsingThumbstickLook && IsUsingGamepad(a_inputDeviceManager);
}

BOOL APIENTRY DllMain(HMODULE a_hModule, DWORD a_ul_reason_for_call, LPVOID a_lpReserved)
{
	if (a_ul_reason_for_call == DLL_PROCESS_ATTACH) {
#ifndef NDEBUG
		while (!IsDebuggerPresent()) {
			Sleep(100);
		}
#endif

		dku::Logger::Init(Plugin::NAME, Plugin::VERSION.string('.'));

		dku::Hook::Trampoline::AllocTrampoline(98);

		// Allow any look input
		{
			auto vtbl = REL::Relocation<std::uintptr_t>(
				RE::Offset::PlayerControls::LookHandler::Vtbl);
			vtbl.write_vfunc(
				1,
				+[](RE::PlayerControls::LookHandler*, RE::InputEvent* event) -> bool {
					if (RE::UserEvents::QLook() != event->QUserEvent()) {
						return false;
					}

					if (event->eventType == RE::INPUT_EVENT_TYPE::MouseMove) {
						UsingThumbstickLook = false;
					} else if (event->eventType == RE::INPUT_EVENT_TYPE::Thumbstick) {
						UsingThumbstickLook = true;
					}

					return true;
				});
		}

		// Prevent left thumbstick from changing device
		{
			auto hook = REL::Relocation<std::uintptr_t>(
				RE::Offset::BSPCGamepadDevice::Poll + 0x2A0);
			REL::safe_fill(hook.address(), REL::NOP, 0x4);
		}

		{
			auto hookLocs = {
				// Fix slow movement on 2 quadrants?
				RE::Offset::PlayerControls::LookHandler::Func10 + 0xE,
				// Fix look sensitivity
				RE::Offset::PlayerControls::Manager::ProcessLookInput + 0x68,
				// Prevent cursor from escaping window
				RE::Offset::Main::Run_WindowsMessageLoop + 0x2F,
				// Fix mouse movement for ship reticle
				RE::Offset::ShipHudDataModel::PerformInputProcessing + 0x7AF,
				RE::Offset::ShipHudDataModel::PerformInputProcessing + 0x82A,
			};

			for (auto offset : hookLocs) {
				auto hook = REL::Relocation<std::uintptr_t>(offset);

				REL::make_pattern<"E8">().match_or_fail(hook.address());
				dku::Hook::write_call<5>(hook.address(), IsUsingThumbstickLook);
			}
		}

		{
			auto hookLocs = {
				// Show cursor for menus
				RE::Offset::IMenu::ShowCursor + 0x14,
				// Use pointer style cursor
				RE::Offset::UI::SetCursorStyle + 0x98,
			};

			for (auto offset : hookLocs) {
				auto hook = REL::Relocation<std::uintptr_t>(offset);

				REL::make_pattern<"E8">().match_or_fail(hook.address());
				dku::Hook::write_call<5>(hook.address(), IsGamepadCursor);
			}
		}
	}

	return TRUE;
}