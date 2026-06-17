#include "pch.h"
#include "MusicPlayer2LyricItem.h"
#include "MusicPlayer2Lyric.h"
#include "DataManager.h"
#include <algorithm>
#include <vector>

#undef min
#undef max

namespace
{
    std::wstring NormalizeText(const std::wstring& text)
    {
        std::wstring result{ text };
        std::replace(result.begin(), result.end(), L'\r', L' ');
        std::replace(result.begin(), result.end(), L'\n', L' ');
        return result;
    }

    std::vector<std::wstring> SplitSupplementText(const std::wstring& text)
    {
        std::vector<std::wstring> lines;
        std::wstring normalized{ NormalizeText(text) };
        size_t start{};
        while (start < normalized.size())
        {
            size_t pos{ normalized.find(L" / ", start) };
            std::wstring line{ normalized.substr(start, pos == std::wstring::npos ? std::wstring::npos : pos - start) };
            if (!line.empty())
                lines.push_back(line);
            if (pos == std::wstring::npos)
                break;
            start = pos + 3;
        }
        return lines;
    }

    std::vector<std::wstring> GetSupplementLines()
    {
        const CMusicPlayer2Lyric& plugin{ CMusicPlayer2Lyric::Instance() };
        if (!plugin.GetCurrentParallelLines().empty())
            return plugin.GetCurrentParallelLines();
        return SplitSupplementText(plugin.GetCurrentTranslate());
    }

    COLORREF SecondaryColor(COLORREF color, bool dark_mode)
    {
        const BYTE r = GetRValue(color);
        const BYTE g = GetGValue(color);
        const BYTE b = GetBValue(color);
        const BYTE target = dark_mode ? 170 : 95;
        return RGB((r + target) / 2, (g + target) / 2, (b + target) / 2);
    }
}

const wchar_t* CMusicPlayer2LyricItem::GetItemName() const
{
    return g_data.StringRes(IDS_PLUGIN_ITEM_NAME);
}

const wchar_t* CMusicPlayer2LyricItem::GetItemId() const
{
    return L"MusicPlayer2Lyric";
}

const wchar_t* CMusicPlayer2LyricItem::GetItemLableText() const
{
    return L"";
}

const wchar_t* CMusicPlayer2LyricItem::GetItemValueText() const
{
    return CMusicPlayer2Lyric::Instance().GetDisplayText().c_str();
}

const wchar_t* CMusicPlayer2LyricItem::GetItemValueSampleText() const
{
    return L"MusicPlayer2 lyric / translation sample text";
}

bool CMusicPlayer2LyricItem::IsCustomDraw() const
{
    return true;
}

int CMusicPlayer2LyricItem::GetItemWidthEx(void* hDC) const
{
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    const CMusicPlayer2Lyric& plugin{ CMusicPlayer2Lyric::Instance() };
    const std::wstring lyric{ NormalizeText(plugin.GetCurrentLyric()) };
    const std::vector<std::wstring> supplement_lines{ GetSupplementLines() };
    int width{ static_cast<int>(pDC->GetTextExtent(GetItemValueSampleText()).cx) };
    if (!lyric.empty())
        width = std::max(width, static_cast<int>(pDC->GetTextExtent(lyric.c_str()).cx));
    for (const auto& line : supplement_lines)
        width = std::max(width, static_cast<int>(pDC->GetTextExtent(line.c_str()).cx));
    width += g_data.DPI(8);
    return std::min(std::max(width, g_data.DPI(g_data.m_setting_data.min_item_width)), g_data.DPI(g_data.m_setting_data.max_item_width));
}

void CMusicPlayer2LyricItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    CRect rect(CPoint(x, y), CSize(w, h));
    rect.DeflateRect(g_data.DPI(4), 0);

    std::wstring lyric{ NormalizeText(CMusicPlayer2Lyric::Instance().GetCurrentLyric()) };
    std::vector<std::wstring> supplement_lines{ GetSupplementLines() };
    if (lyric.empty())
        lyric = CMusicPlayer2Lyric::Instance().GetDisplayText();

    const int old_bk_mode = pDC->SetBkMode(TRANSPARENT);
    const COLORREF old_text_color = pDC->GetTextColor();
    const COLORREF text_color = old_text_color;
    const UINT text_flags = DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
    CFont small_font;
    CFont* old_font = nullptr;
    LOGFONT log_font{};
    CFont* current_font = pDC->GetCurrentFont();
    if (current_font != nullptr && current_font->GetLogFont(&log_font))
    {
        log_font.lfHeight = log_font.lfHeight * g_data.m_setting_data.supplement_font_percent / 100;
        small_font.CreateFontIndirect(&log_font);
    }

    if (g_data.m_setting_data.max_display_lines >= 3 && supplement_lines.size() >= 2 && h >= g_data.DPI(38))
    {
        const int line_height{ rect.Height() / 3 };
        CRect lyric_rect{ rect };
        lyric_rect.bottom = rect.top + line_height;
        CRect second_rect{ rect };
        second_rect.top = lyric_rect.bottom;
        second_rect.bottom = second_rect.top + line_height;
        CRect third_rect{ rect };
        third_rect.top = second_rect.bottom;

        pDC->SetTextColor(text_color);
        pDC->DrawText(lyric.c_str(), lyric_rect, text_flags | DT_BOTTOM);
        pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
        if (small_font.GetSafeHandle() != nullptr)
            old_font = pDC->SelectObject(&small_font);
        pDC->DrawText(supplement_lines[0].c_str(), second_rect, text_flags | DT_VCENTER);
        pDC->DrawText(supplement_lines[1].c_str(), third_rect, text_flags | DT_TOP);
        if (old_font != nullptr)
            pDC->SelectObject(old_font);
    }
    else if (g_data.m_setting_data.max_display_lines >= 2 && !supplement_lines.empty() && h >= g_data.DPI(28))
    {
        CRect lyric_rect{ rect };
        lyric_rect.bottom = rect.top + rect.Height() / 2;
        CRect translate_rect{ rect };
        translate_rect.top = lyric_rect.bottom;
        std::wstring translate{ supplement_lines[0] };
        for (size_t i{ 1 }; i < supplement_lines.size(); ++i)
        {
            translate += L" / ";
            translate += supplement_lines[i];
        }

        pDC->SetTextColor(text_color);
        pDC->DrawText(lyric.c_str(), lyric_rect, text_flags | DT_BOTTOM);
        pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
        pDC->DrawText(translate.c_str(), translate_rect, text_flags | DT_TOP);
    }
    else
    {
        for (const auto& line : supplement_lines)
        {
            lyric += L" / ";
            lyric += line;
        }
        pDC->SetTextColor(text_color);
        pDC->DrawText(lyric.c_str(), rect, text_flags | DT_VCENTER);
    }

    pDC->SetTextColor(old_text_color);
    pDC->SetBkMode(old_bk_mode);
}
