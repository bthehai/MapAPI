#pragma once


class GLAdapter {
public:
	static bool GDX_DESKTOP_QUIRKS;
	static bool GDX_WEBGL_QUIRKS;
	static bool NO_BUFFER_SUB_DATA;
};

__declspec(selectany) bool GLAdapter::GDX_DESKTOP_QUIRKS = false;

__declspec(selectany) bool GLAdapter::GDX_WEBGL_QUIRKS = false;

__declspec(selectany) bool GLAdapter::NO_BUFFER_SUB_DATA = false;
