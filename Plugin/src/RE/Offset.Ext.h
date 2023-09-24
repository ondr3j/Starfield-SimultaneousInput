#pragma once

#include "REL/Relocation.h"

namespace RE
{
	namespace Offset
	{
		namespace BSInputDeviceManager
		{
			constexpr REL::Offset IsUsingGamepad{ 0x2C90070 };
		}

		namespace BSPCGamepadDevice
		{
			constexpr REL::Offset Poll{ 0x2CAEF3C };
		}

		namespace IMenu
		{
			constexpr REL::Offset ShowCursor{ 0x2E77DB0 };
		}

		namespace Main
		{
			constexpr REL::Offset Run_WindowsMessageLoop{ 0x23F53A4 };
		}

		namespace PlayerControls
		{
			namespace LookHandler
			{
				constexpr REL::Offset Vtbl{ 0x4485150 };
				constexpr REL::Offset Func10{ 0x1F45650 };
			}

			namespace Manager
			{
				constexpr REL::Offset ProcessLookInput{ 0x1F4CE28 };
			}
		}

		namespace ShipHudDataModel
		{
			constexpr REL::Offset PerformInputProcessing{ 0x20FB698 };
		}

		namespace UI
		{
			constexpr REL::Offset SetCursorStyle{ 0x2E6E6F4 };
		}
	}
}
