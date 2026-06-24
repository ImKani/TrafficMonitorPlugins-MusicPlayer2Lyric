# MusicPlayer2Lyric 插件 v1.1.2

这是 `v1.1.1` 的界面补丁版本。

## 主要变化

- 修正中文资源下插件设置窗口没有显示“并列歌词行”选项的问题。
- 英文资源仍显示为 `Parallel lyric line`。
- 功能行为与 `v1.1.1` 一致：非强制三行时可用 `parallel_lyric_line` 选择末行或指定副行。

## 配置说明

```ini
parallel_lyric_line=0
```

- `0` 或 `-1`：显示末行。
- `1` 到 `3`：显示指定副行。

## 相关仓库

- 修改版 MusicPlayer2：`https://github.com/ImKani/MusicPlayer2-TaskbarLyric`
- 整合发布仓库：`https://github.com/ImKani/MusicPlayer2TaskbarLyric`

## 版权

Copyright (C) 2026 by ImKani.
