#pragma once

#include "PluginInterface.h"
#include "MusicPlayer2LyricItem.h"
#include "Mp2LyricIpcReader.h"
#include <string>
#include <vector>

class CMusicPlayer2Lyric : public ITMPlugin
{
private:
    CMusicPlayer2Lyric();

public:
    static CMusicPlayer2Lyric& Instance();

    virtual IPluginItem* GetItem(int index) override;
    virtual const wchar_t* GetTooltipInfo() override;
    virtual void DataRequired() override;
    virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
    virtual void OnExtenedInfo(ExtendedInfoIndex index, const wchar_t* data) override;

    const std::wstring& GetDisplayText() const { return m_display_text; }
    const std::wstring& GetCurrentLyric() const { return m_current_lyric; }
    const std::wstring& GetCurrentTranslate() const { return m_current_translate; }
    const std::vector<std::wstring>& GetCurrentParallelLines() const { return m_current_parallel_lines; }

private:
    static CMusicPlayer2Lyric m_instance;
    CMusicPlayer2LyricItem m_item;
    CMp2LyricIpcReader m_reader;
    DWORD m_last_sequence{};
    std::wstring m_display_text;
    std::wstring m_current_lyric;
    std::wstring m_current_translate;
    std::vector<std::wstring> m_current_parallel_lines;
    std::vector<std::wstring> m_next_parallel_lines;
    std::wstring m_tooltip_info;
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
