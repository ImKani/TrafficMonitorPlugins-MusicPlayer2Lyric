#include "pch.h"
#include "Mp2LyricIpcReader.h"

CMp2LyricIpcReader::~CMp2LyricIpcReader()
{
    Close();
}

bool CMp2LyricIpcReader::EnsureOpen()
{
    if (m_state != nullptr)
        return true;

    m_mapping = OpenFileMappingW(FILE_MAP_READ, FALSE, MP2_LYRIC_IPC_NAME);
    if (m_mapping == nullptr)
        return false;

    m_state = static_cast<const Mp2LyricIpcState*>(
        MapViewOfFile(m_mapping, FILE_MAP_READ, 0, 0, sizeof(Mp2LyricIpcState)));
    if (m_state == nullptr)
    {
        CloseHandle(m_mapping);
        m_mapping = nullptr;
        return false;
    }

    return true;
}

bool CMp2LyricIpcReader::Read(Mp2LyricIpcState& state)
{
    if (!EnsureOpen())
        return false;

    state = *m_state;
    if (state.magic != MP2_LYRIC_IPC_MAGIC || state.version != MP2_LYRIC_IPC_VERSION)
    {
        Close();
        return false;
    }

    return true;
}

void CMp2LyricIpcReader::Close()
{
    if (m_state != nullptr)
    {
        UnmapViewOfFile(m_state);
        m_state = nullptr;
    }
    if (m_mapping != nullptr)
    {
        CloseHandle(m_mapping);
        m_mapping = nullptr;
    }
}
