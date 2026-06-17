# MusicPlayer2Lyric 插件 v1.1.0

这是用于 TrafficMonitor 的 MusicPlayer2 任务栏歌词插件，需要配合修改版 MusicPlayer2 使用。

普通用户建议从整合发布仓库下载完整包：

```text
https://github.com/ImKani/MusicPlayer2TaskbarLyric
```

## 主要变化

- 新增 `MusicPlayer2Lyric` 插件工程。
- 通过共享内存读取修改版 MusicPlayer2 发布的歌词状态。
- 支持显示曲名、歌手、当前歌词、下一句歌词、翻译和同时间戳并列歌词行。
- 支持最多三行显示和强制三行显示。
- 支持曲名/歌手信息区，曲名与艺术家分两行显示。
- 支持“当前歌词 + 下一句歌词”模式，启用后强制不显示翻译、罗马音或其他并列行。
- 增加插件设置窗口，可在 TrafficMonitor 插件管理中修改显示行数、字号比例、宽度和显示模式。
- 插件版本更新为 `1.1.0`。
- 插件元数据增加仓库链接和 `Copyright (C) 2026 by ImKani`。

## 文件

- `MusicPlayer2Lyric.dll`
- `MusicPlayer2Lyric.ini.example`

## 相关仓库

- 修改版 MusicPlayer2：`https://github.com/ImKani/MusicPlayer2-TaskbarLyric`
- 整合发布仓库：`https://github.com/ImKani/MusicPlayer2TaskbarLyric`

## 版权

Copyright (C) 2026 by ImKani.

本插件基于 `zhongyang219/TrafficMonitorPlugins`，遵循上游 MIT License。
