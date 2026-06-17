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

    COLORREF DividerColor(COLORREF color, bool dark_mode)
    {
        const BYTE target = dark_mode ? 95 : 185;
        return RGB((GetRValue(color) + target) / 2, (GetGValue(color) + target) / 2, (GetBValue(color) + target) / 2);
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
    const std::wstring title{ NormalizeText(plugin.GetTitle()) };
    const std::wstring artist{ NormalizeText(plugin.GetArtist()) };
    const std::vector<std::wstring> supplement_lines{ GetSupplementLines() };
    int width{ static_cast<int>(pDC->GetTextExtent(GetItemValueSampleText()).cx) };
    if (!lyric.empty())
        width = std::max(width, static_cast<int>(pDC->GetTextExtent(lyric.c_str()).cx));
    if (g_data.m_setting_data.show_song_info && (!title.empty() || !artist.empty()))
    {
        const int info_width{ std::max(static_cast<int>(pDC->GetTextExtent(title.c_str()).cx),
            static_cast<int>(pDC->GetTextExtent(artist.c_str()).cx)) };
        width = std::max(width, info_width + g_data.DPI(90));
    }
    if (g_data.m_setting_data.show_current_next && !plugin.GetNextLyric().empty())
        width = std::max(width, static_cast<int>(pDC->GetTextExtent(plugin.GetNextLyric().c_str()).cx));
    if (!g_data.m_setting_data.show_current_next)
    {
        for (const auto& line : supplement_lines)
            width = std::max(width, static_cast<int>(pDC->GetTextExtent(line.c_str()).cx));
    }
    width += g_data.DPI(8);
    return std::min(std::max(width, g_data.DPI(g_data.m_setting_data.min_item_width)), g_data.DPI(g_data.m_setting_data.max_item_width));
}

void CMusicPlayer2LyricItem::DrawItem(void* hDC, int x, int y, int w, int h, bool dark_mode)
{
    CDC* pDC = CDC::FromHandle((HDC)hDC);
    CRect rect(CPoint(x, y), CSize(w, h));
    rect.DeflateRect(g_data.DPI(4), 0);

    const CMusicPlayer2Lyric& plugin{ CMusicPlayer2Lyric::Instance() };
    std::wstring lyric{ NormalizeText(plugin.GetCurrentLyric()) };
    std::vector<std::wstring> supplement_lines{ GetSupplementLines() };
    if (lyric.empty())
        lyric = plugin.GetDisplayText();

    const std::wstring title{ NormalizeText(plugin.GetTitle()) };
    const std::wstring artist{ NormalizeText(plugin.GetArtist()) };
    std::wstring next_lyric{ NormalizeText(plugin.GetNextLyric()) };
    if (next_lyric.empty())
        next_lyric = NormalizeText(plugin.GetNextTranslate());

    const int old_bk_mode = pDC->SetBkMode(TRANSPARENT);
    const COLORREF old_text_color = pDC->GetTextColor();
    const COLORREF text_color = old_text_color;
    const UINT text_flags = DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
    CFont primary_font;
    CFont supplement_font;
    LOGFONT log_font{};
    CFont* current_font = pDC->GetCurrentFont();
    if (current_font != nullptr && current_font->GetLogFont(&log_font))
    {
        LOGFONT primary_log_font{ log_font };
        primary_log_font.lfHeight = primary_log_font.lfHeight * g_data.m_setting_data.primary_font_percent / 100;
        primary_font.CreateFontIndirect(&primary_log_font);

        LOGFONT supplement_log_font{ log_font };
        supplement_log_font.lfHeight = supplement_log_font.lfHeight * g_data.m_setting_data.supplement_font_percent / 100;
        supplement_font.CreateFontIndirect(&supplement_log_font);
    }

    CRect content_rect{ rect };
    if (g_data.m_setting_data.show_song_info && (!title.empty() || !artist.empty()) && rect.Width() >= g_data.DPI(180))
    {
        CFont* old_font = nullptr;
        if (supplement_font.GetSafeHandle() != nullptr)
            old_font = pDC->SelectObject(&supplement_font);

        int info_width{ std::max(static_cast<int>(pDC->GetTextExtent(title.c_str()).cx),
            static_cast<int>(pDC->GetTextExtent(artist.c_str()).cx)) + g_data.DPI(14) };
        info_width = std::min(info_width, std::max(g_data.DPI(96), rect.Width() * 2 / 5));
        CRect info_rect{ rect };
        info_rect.right = info_rect.left + info_width;
        CRect divider_rect{ rect };
        divider_rect.left = info_rect.right + g_data.DPI(4);
        divider_rect.right = divider_rect.left + g_data.DPI(1);
        content_rect.left = divider_rect.right + g_data.DPI(8);

        CRect info_text_rect{ info_rect };
        info_text_rect.DeflateRect(g_data.DPI(2), 0);
        pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
        if (!title.empty() && !artist.empty() && info_text_rect.Height() >= g_data.DPI(28))
        {
            CRect title_rect{ info_text_rect };
            title_rect.bottom = info_text_rect.top + info_text_rect.Height() / 2;
            CRect artist_rect{ info_text_rect };
            artist_rect.top = title_rect.bottom;
            pDC->SetTextColor(text_color);
            pDC->DrawText(title.c_str(), title_rect, text_flags | DT_BOTTOM);
            pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
            pDC->DrawText(artist.c_str(), artist_rect, text_flags | DT_TOP);
        }
        else
        {
            const std::wstring info_text{ !title.empty() ? title : artist };
            pDC->DrawText(info_text.c_str(), info_text_rect, text_flags | DT_VCENTER);
        }
        CPen divider_pen(PS_SOLID, g_data.DPI(1), DividerColor(text_color, dark_mode));
        CPen* old_pen = pDC->SelectObject(&divider_pen);
        const int divider_top{ rect.top + g_data.DPI(4) };
        const int divider_bottom{ rect.bottom - g_data.DPI(4) };
        pDC->MoveTo(divider_rect.left, divider_top);
        pDC->LineTo(divider_rect.left, divider_bottom);
        if (old_pen != nullptr)
            pDC->SelectObject(old_pen);
        if (old_font != nullptr)
            pDC->SelectObject(old_font);
    }

    if (content_rect.Width() <= g_data.DPI(24))
        content_rect = rect;

    if (g_data.m_setting_data.show_current_next)
    {
        if (!next_lyric.empty() && h >= g_data.DPI(28))
        {
            CRect lyric_rect{ content_rect };
            lyric_rect.bottom = content_rect.top + content_rect.Height() / 2;
            CRect next_rect{ content_rect };
            next_rect.top = lyric_rect.bottom;

            pDC->SetTextColor(text_color);
            pDC->DrawText(lyric.c_str(), lyric_rect, text_flags | DT_BOTTOM);
            pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
            CFont* old_font = nullptr;
            if (supplement_font.GetSafeHandle() != nullptr)
                old_font = pDC->SelectObject(&supplement_font);
            pDC->DrawText(next_lyric.c_str(), next_rect, text_flags | DT_TOP);
            if (old_font != nullptr)
                pDC->SelectObject(old_font);
        }
        else
        {
            pDC->SetTextColor(text_color);
            pDC->DrawText(lyric.c_str(), content_rect, text_flags | DT_VCENTER);
        }

        pDC->SetTextColor(old_text_color);
        pDC->SetBkMode(old_bk_mode);
        return;
    }

    const bool draw_three_lines{ g_data.m_setting_data.max_display_lines >= 3
        && supplement_lines.size() >= 2
        && (g_data.m_setting_data.force_three_lines || h >= g_data.DPI(38)) };

    if (draw_three_lines)
    {
        const int line_height{ content_rect.Height() / 3 };
        CRect lyric_rect{ content_rect };
        lyric_rect.bottom = content_rect.top + line_height;
        CRect second_rect{ content_rect };
        second_rect.top = lyric_rect.bottom;
        second_rect.bottom = second_rect.top + line_height;
        CRect third_rect{ content_rect };
        third_rect.top = second_rect.bottom;

        CFont* old_font = nullptr;
        pDC->SetTextColor(text_color);
        if (primary_font.GetSafeHandle() != nullptr)
            old_font = pDC->SelectObject(&primary_font);
        pDC->DrawText(lyric.c_str(), lyric_rect, text_flags | DT_BOTTOM);
        if (old_font != nullptr)
        {
            pDC->SelectObject(old_font);
            old_font = nullptr;
        }

        pDC->SetTextColor(SecondaryColor(text_color, dark_mode));
        if (supplement_font.GetSafeHandle() != nullptr)
            old_font = pDC->SelectObject(&supplement_font);
        pDC->DrawText(supplement_lines[0].c_str(), second_rect, text_flags | DT_VCENTER);
        pDC->DrawText(supplement_lines[1].c_str(), third_rect, text_flags | DT_TOP);
        if (old_font != nullptr)
            pDC->SelectObject(old_font);
    }
    else if (g_data.m_setting_data.max_display_lines >= 2 && !supplement_lines.empty() && h >= g_data.DPI(28))
    {
        CRect lyric_rect{ content_rect };
        lyric_rect.bottom = content_rect.top + content_rect.Height() / 2;
        CRect translate_rect{ content_rect };
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
        CFont* old_font = nullptr;
        if (supplement_font.GetSafeHandle() != nullptr)
            old_font = pDC->SelectObject(&supplement_font);
        pDC->DrawText(translate.c_str(), translate_rect, text_flags | DT_TOP);
        if (old_font != nullptr)
            pDC->SelectObject(old_font);
    }
    else
    {
        for (const auto& line : supplement_lines)
        {
            lyric += L" / ";
            lyric += line;
        }
        pDC->SetTextColor(text_color);
        pDC->DrawText(lyric.c_str(), content_rect, text_flags | DT_VCENTER);
    }

    pDC->SetTextColor(old_text_color);
    pDC->SetBkMode(old_bk_mode);
}
