#include "pch.h"
#include "DataManager.h"

CDataManager CDataManager::m_instance;

CDataManager::CDataManager()
{
    //初始化DPI
    HDC hDC = ::GetDC(HWND_DESKTOP);
    m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
    ::ReleaseDC(HWND_DESKTOP, hDC);
}

CDataManager::~CDataManager()
{
    SaveConfig();
}

CDataManager& CDataManager::Instance()
{
    return m_instance;
}

static void WritePrivateProfileInt(const wchar_t* app_name, const wchar_t* key_name, int value, const wchar_t* file_path)
{
    wchar_t buff[16];
    swprintf_s(buff, L"%d", value);
    WritePrivateProfileString(app_name, key_name, buff, file_path);
}

static int ClampInt(int value, int min_value, int max_value)
{
    return min(max(value, min_value), max_value);
}

void CDataManager::LoadConfig(const std::wstring& config_dir)
{
    m_config_path = config_dir + L"MusicPlayer2Lyric.ini";
    m_setting_data.max_display_lines = ClampInt(GetPrivateProfileInt(L"config", L"max_display_lines", 3, m_config_path.c_str()), 1, 3);
    m_setting_data.supplement_font_percent = ClampInt(GetPrivateProfileInt(L"config", L"supplement_font_percent", 80, m_config_path.c_str()), 50, 100);
    m_setting_data.min_item_width = ClampInt(GetPrivateProfileInt(L"config", L"min_item_width", 120, m_config_path.c_str()), 40, 1000);
    m_setting_data.max_item_width = ClampInt(GetPrivateProfileInt(L"config", L"max_item_width", 360, m_config_path.c_str()), m_setting_data.min_item_width, 2000);
    SaveConfig();
}

void CDataManager::SaveConfig() const
{
    if (m_config_path.empty())
        return;

    WritePrivateProfileInt(L"config", L"max_display_lines", m_setting_data.max_display_lines, m_config_path.c_str());
    WritePrivateProfileInt(L"config", L"supplement_font_percent", m_setting_data.supplement_font_percent, m_config_path.c_str());
    WritePrivateProfileInt(L"config", L"min_item_width", m_setting_data.min_item_width, m_config_path.c_str());
    WritePrivateProfileInt(L"config", L"max_item_width", m_setting_data.max_item_width, m_config_path.c_str());
}

const CString& CDataManager::StringRes(UINT id)
{
    auto iter = m_string_table.find(id);
    if (iter != m_string_table.end())
    {
        return iter->second;
    }
    else
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        m_string_table[id].LoadString(id);
        return m_string_table[id];
    }
}

void CDataManager::DPIFromWindow(CWnd* pWnd)
{
    CWindowDC dc(pWnd);
    HDC hDC = dc.GetSafeHdc();
    m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);
}

int CDataManager::DPI(int pixel)
{
    return m_dpi * pixel / 96;
}

float CDataManager::DPIF(float pixel)
{
    return m_dpi * pixel / 96;
}

int CDataManager::RDPI(int pixel)
{
    return pixel * 96 / m_dpi;
}

HICON CDataManager::GetIcon(UINT id)
{
    auto iter = m_icons.find(id);
    if (iter != m_icons.end())
    {
        return iter->second;
    }
    else
    {
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
        HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(id), IMAGE_ICON, DPI(16), DPI(16), 0);
        m_icons[id] = hIcon;
        return hIcon;
    }
}
