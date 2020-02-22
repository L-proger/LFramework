#pragma once

#include <type_traits>

#define LF_TERMINAL_ANSI_ESC "\033"

#define LF_TERMINAL_ANSI_SS2 LF_TERMINAL_ANSI_ESC"N"
#define LF_TERMINAL_ANSI_SS3 LF_TERMINAL_ANSI_ESC"O"
#define LF_TERMINAL_ANSI_DCS LF_TERMINAL_ANSI_ESC"P"
#define LF_TERMINAL_ANSI_CSI LF_TERMINAL_ANSI_ESC"["
#define LF_TERMINAL_ANSI_ST  LF_TERMINAL_ANSI_ESC"\\"
#define LF_TERMINAL_ANSI_OSC LF_TERMINAL_ANSI_ESC"]"
#define LF_TERMINAL_ANSI_SOS LF_TERMINAL_ANSI_ESC"X"
#define LF_TERMINAL_ANSI_PM  LF_TERMINAL_ANSI_ESC"^"
#define LF_TERMINAL_ANSI_APC LF_TERMINAL_ANSI_ESC"_"
#define LF_TERMINAL_ANSI_RIS LF_TERMINAL_ANSI_ESC"c"

#if defined(LF__TERMINAL_ANSI_CODES_ENABLE)
#define LF_TERMINAL_ANSI_CODE(__id, __suffix) LF_TERMINAL_ANSI_CSI #__id #__suffix
#else
#define LF_TERMINAL_ANSI_CODE(__id, __suffix) ""
#endif

#define LF_TERMINAL_ANSI_GRAPHICS_CODE(__id)  LF_TERMINAL_ANSI_CODE(__id, m)
#define LF_TERMINAL_ANSI_CODE_DEFINE(__codeName, __id) inline const char (& __codeName ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_GRAPHICS_CODE(__id))>>::value] { return LF_TERMINAL_ANSI_GRAPHICS_CODE(__id); }


namespace LFramework::Terminal::Ansi {
	namespace Color {

		LF_TERMINAL_ANSI_CODE_DEFINE(Black, 30)
		LF_TERMINAL_ANSI_CODE_DEFINE(Red, 31)
		LF_TERMINAL_ANSI_CODE_DEFINE(Green, 32)
		LF_TERMINAL_ANSI_CODE_DEFINE(Yellow, 33)
		LF_TERMINAL_ANSI_CODE_DEFINE(Blue, 34)
		LF_TERMINAL_ANSI_CODE_DEFINE(Magenta, 35)
		LF_TERMINAL_ANSI_CODE_DEFINE(Cyan, 36)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightGray, 37)
		LF_TERMINAL_ANSI_CODE_DEFINE(DarkGray, 90)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightRed, 91)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightGreen, 92)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightYellow, 93)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightBlue, 94)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightMagenta, 95)
		LF_TERMINAL_ANSI_CODE_DEFINE(LightCyan, 96)
		LF_TERMINAL_ANSI_CODE_DEFINE(White, 97)

		//Background
		namespace BG {
			LF_TERMINAL_ANSI_CODE_DEFINE(Black, 40)
			LF_TERMINAL_ANSI_CODE_DEFINE(Red, 41)
			LF_TERMINAL_ANSI_CODE_DEFINE(Green, 42)
			LF_TERMINAL_ANSI_CODE_DEFINE(Yellow, 43)
			LF_TERMINAL_ANSI_CODE_DEFINE(Blue, 44)
			LF_TERMINAL_ANSI_CODE_DEFINE(Magenta, 45)
			LF_TERMINAL_ANSI_CODE_DEFINE(Cyan, 46)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightGray, 47)
			LF_TERMINAL_ANSI_CODE_DEFINE(DarkGray, 100)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightRed, 101)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightGreen, 102)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightYellow, 103)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightBlue, 104)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightMagenta, 105)
			LF_TERMINAL_ANSI_CODE_DEFINE(LightCyan, 106)
			LF_TERMINAL_ANSI_CODE_DEFINE(White, 107)
		}
	}

	namespace Format {
		LF_TERMINAL_ANSI_CODE_DEFINE(BoldSet, 1)
		LF_TERMINAL_ANSI_CODE_DEFINE(BoldReset, 21)
		LF_TERMINAL_ANSI_CODE_DEFINE(UnderlinedSet, 4)
		LF_TERMINAL_ANSI_CODE_DEFINE(UnderlinedReset, 24)
		LF_TERMINAL_ANSI_CODE_DEFINE(BlinkSet, 5)
		LF_TERMINAL_ANSI_CODE_DEFINE(BlinkReset, 25)
		LF_TERMINAL_ANSI_CODE_DEFINE(ReverseSet, 7)
		LF_TERMINAL_ANSI_CODE_DEFINE(ReverseReset, 27)
		LF_TERMINAL_ANSI_CODE_DEFINE(ResetAll, 0)

		LF_TERMINAL_ANSI_CODE_DEFINE(Framed, 51)
		LF_TERMINAL_ANSI_CODE_DEFINE(Encircled, 52)
		LF_TERMINAL_ANSI_CODE_DEFINE(Overlined, 53)
		LF_TERMINAL_ANSI_CODE_DEFINE(ClearFramedEncircled, 54)
		LF_TERMINAL_ANSI_CODE_DEFINE(ClearOverlined, 55)
	}


	namespace Cursor {
		inline const char (& EnableBlinking ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"?12h")>>::value] {
			return LF_TERMINAL_ANSI_CSI"?12h";
		}
		inline const char (& DisableBlinking ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"?12l")>>::value] {
			return LF_TERMINAL_ANSI_CSI"?12l";
		}
		inline const char (& Show ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"?25h")>>::value] {
			return LF_TERMINAL_ANSI_CSI"?25h";
		}
		inline const char (& Hide ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"?25l")>>::value] {
			return LF_TERMINAL_ANSI_CSI"?25l";
		}


		inline const char (& MoveHome ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"H")>>::value] {
			return LF_TERMINAL_ANSI_CSI"H";
		}

		inline const char (& SavePosition ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"s")>>::value] {
			return LF_TERMINAL_ANSI_CSI"s";
		}

		inline const char (& RestorePosition ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"u")>>::value] {
			return LF_TERMINAL_ANSI_CSI"u";
		}
	}

	namespace TextEdit {
		inline const char (& EraseLineCurToBeg ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"K")>>::value] {
			return LF_TERMINAL_ANSI_CSI"K";
		}
		inline const char (& EraseLineBegToCur ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"1K")>>::value] {
			return LF_TERMINAL_ANSI_CSI"1K";
		}
		inline const char (& EraseLine ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"2K")>>::value] {
			return LF_TERMINAL_ANSI_CSI"2K";
		}
	}

	namespace Viewport {
		inline const char (& ClearScreenAfterCursor ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"0J")>>::value] {
			return LF_TERMINAL_ANSI_CSI"0J";
		}
		inline const char (& ClearScreenBeforeCursor ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"1J")>>::value] {
			return LF_TERMINAL_ANSI_CSI"1J";
		}
		inline const char (& ClearScreen ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"2J")>>::value] {
			return LF_TERMINAL_ANSI_CSI"2J";
		}
		inline const char (& ClearScreenAndScrollback ())[std::extent<std::remove_reference_t<decltype(LF_TERMINAL_ANSI_CSI"3J")>>::value] {
			return LF_TERMINAL_ANSI_CSI"3J";
		}
	}
}
