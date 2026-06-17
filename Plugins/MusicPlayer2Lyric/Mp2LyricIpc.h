#pragma once

#include <Windows.h>

constexpr wchar_t MP2_LYRIC_IPC_NAME[] = L"Local\\MusicPlayer2_TaskbarLyric_State";
constexpr DWORD MP2_LYRIC_IPC_MAGIC = 0x324C504D; // MPL2
constexpr DWORD MP2_LYRIC_IPC_VERSION = 3;

struct Mp2LyricIpcState
{
    DWORD magic{};
    DWORD version{};
    DWORD sequence{};
    DWORD process_id{};
    ULONGLONG tick_count{};
    BOOL running{};
    BOOL playing{};
    BOOL has_lyric{};
    int progress{};
    wchar_t title[256]{};
    wchar_t artist[256]{};
    wchar_t current_lyric[512]{};
    wchar_t current_translate[512]{};
    wchar_t next_lyric[512]{};
    wchar_t next_translate[512]{};
    DWORD current_parallel_count{};
    wchar_t current_parallel_1[512]{};
    wchar_t current_parallel_2[512]{};
    wchar_t current_parallel_3[512]{};
    DWORD next_parallel_count{};
    wchar_t next_parallel_1[512]{};
    wchar_t next_parallel_2[512]{};
    wchar_t next_parallel_3[512]{};
};
