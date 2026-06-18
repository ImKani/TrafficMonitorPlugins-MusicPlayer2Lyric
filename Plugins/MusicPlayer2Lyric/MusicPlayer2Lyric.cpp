#include "pch.h"
#include "MusicPlayer2Lyric.h"
#include "DataManager.h"
#include "OptionsDlg.h"

CMusicPlayer2Lyric CMusicPlayer2Lyric::m_instance;

namespace
{
    void AppendIpcText(std::vector<std::wstring>& lines, const wchar_t* text)
    {
        if (text != nullptr && text[0] != L'\0')
            lines.emplace_back(text);
    }

    void ReadParallelLines(std::vector<std::wstring>& lines, DWORD count, const wchar_t* line1,
        const wchar_t* line2, const wchar_t* line3)
    {
        lines.clear();
        if (count > 0)
            AppendIpcText(lines, line1);
        if (count > 1)
            AppendIpcText(lines, line2);
        if (count > 2)
            AppendIpcText(lines, line3);
    }
}

CMusicPlayer2Lyric::CMusicPlayer2Lyric()
{
}

CMusicPlayer2Lyric& CMusicPlayer2Lyric::Instance()
{
    return m_instance;
}

IPluginItem* CMusicPlayer2Lyric::GetItem(int index)
{
    switch (index)
    {
    case 0:
        return &m_item;
    default:
        break;
    }
    return nullptr;
}

const wchar_t* CMusicPlayer2Lyric::GetTooltipInfo()
{
    return m_tooltip_info.c_str();
}

void CMusicPlayer2Lyric::DataRequired()
{
    Mp2LyricIpcState state{};
    if (!m_reader.Read(state))
    {
        m_last_sequence = 0;
        m_display_text.clear();
        m_title.clear();
        m_artist.clear();
        m_current_lyric.clear();
        m_current_translate.clear();
        m_next_lyric.clear();
        m_next_translate.clear();
        m_current_parallel_lines.clear();
        m_next_parallel_lines.clear();
        m_tooltip_info = L"MusicPlayer2 is not running.";
        return;
    }

    if (state.sequence == m_last_sequence)
        return;

    m_last_sequence = state.sequence;
    if (!state.running)
    {
        m_display_text.clear();
        m_title.clear();
        m_artist.clear();
        m_current_lyric.clear();
        m_current_translate.clear();
        m_next_lyric.clear();
        m_next_translate.clear();
        m_current_parallel_lines.clear();
        m_next_parallel_lines.clear();
        m_tooltip_info = L"MusicPlayer2 is not running.";
        return;
    }

    ReadParallelLines(m_current_parallel_lines, state.current_parallel_count,
        state.current_parallel_1, state.current_parallel_2, state.current_parallel_3);
    ReadParallelLines(m_next_parallel_lines, state.next_parallel_count,
        state.next_parallel_1, state.next_parallel_2, state.next_parallel_3);

    m_title = state.title;
    m_artist = state.artist;
    m_next_lyric = state.next_lyric;
    m_next_translate = state.next_translate;

    if (state.has_lyric && state.current_lyric[0] != L'\0')
    {
        m_current_lyric = state.current_lyric;
        m_current_translate = state.current_translate;
    }
    else
    {
        m_current_lyric = state.title;
        m_current_translate.clear();
        m_current_parallel_lines.clear();
        m_next_parallel_lines.clear();
    }
    m_display_text = m_current_lyric;
    if (!m_current_parallel_lines.empty())
    {
        for (const auto& line : m_current_parallel_lines)
        {
            m_display_text += L"\r\n";
            m_display_text += line;
        }
    }
    else if (!m_current_translate.empty())
    {
        m_display_text += L"\r\n";
        m_display_text += m_current_translate;
    }

    m_tooltip_info.clear();
    if (state.title[0] != L'\0')
        m_tooltip_info += state.title;
    if (state.artist[0] != L'\0')
    {
        if (!m_tooltip_info.empty())
            m_tooltip_info += L" - ";
        m_tooltip_info += state.artist;
    }
    if (state.next_lyric[0] != L'\0')
    {
        if (!m_tooltip_info.empty())
            m_tooltip_info += L"\r\n";
        m_tooltip_info += L"Next: ";
        m_tooltip_info += state.next_lyric;
        if (!m_next_parallel_lines.empty())
        {
            for (const auto& line : m_next_parallel_lines)
            {
                m_tooltip_info += L"\r\n";
                m_tooltip_info += line;
            }
        }
        else if (state.next_translate[0] != L'\0')
        {
            m_tooltip_info += L"\r\n";
            m_tooltip_info += state.next_translate;
        }
    }
}

ITMPlugin::OptionReturn CMusicPlayer2Lyric::ShowOptionsDialog(void* hParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CWnd* pParent = CWnd::FromHandle((HWND)hParent);
    COptionsDlg dlg(pParent);
    dlg.m_data = g_data.m_setting_data;
    if (dlg.DoModal() == IDOK)
    {
        g_data.m_setting_data = dlg.m_data;
        g_data.SaveConfig();
        return ITMPlugin::OR_OPTION_CHANGED;
    }
    return ITMPlugin::OR_OPTION_UNCHANGED;
}

const wchar_t* CMusicPlayer2Lyric::GetInfo(PluginInfoIndex index)
{
    switch (index)
    {
    case TMI_NAME:
        return g_data.StringRes(IDS_PLUGIN_NAME).GetString();
    case TMI_DESCRIPTION:
        return g_data.StringRes(IDS_PLUGIN_DESCRIPTION).GetString();
    case TMI_AUTHOR:
        return L"Kani";
    case TMI_COPYRIGHT:
        return L"Copyright (C) 2026 by ImKani";
    case ITMPlugin::TMI_URL:
        return L"https://github.com/ImKani/TrafficMonitorPlugins-MusicPlayer2Lyric";
    case TMI_VERSION:
        return L"1.1.1";
    default:
        break;
    }
    return L"";
}

void CMusicPlayer2Lyric::OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data)
{
    switch (index)
    {
    case ITMPlugin::EI_CONFIG_DIR:
        g_data.LoadConfig(std::wstring(data));
        break;
    default:
        break;
    }
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CMusicPlayer2Lyric::Instance();
}
