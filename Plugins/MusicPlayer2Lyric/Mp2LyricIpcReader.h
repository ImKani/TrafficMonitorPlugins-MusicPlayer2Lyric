#pragma once

#include "Mp2LyricIpc.h"

class CMp2LyricIpcReader
{
public:
    ~CMp2LyricIpcReader();

    bool Read(Mp2LyricIpcState& state);
    void Close();

private:
    bool EnsureOpen();

private:
    HANDLE m_mapping{};
    const Mp2LyricIpcState* m_state{};
};
