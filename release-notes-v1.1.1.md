# MusicPlayer2Lyric 插件 v1.1.1

这是一个插件侧补丁版本，用于修正非强制三行显示时同时间戳多行歌词的选择行为。

普通用户建议从整合发布仓库下载完整包：

```text
https://github.com/ImKani/MusicPlayer2TaskbarLyric
```

## 主要变化

- 新增 `parallel_lyric_line` 配置项。
- 插件设置窗口新增“Parallel lyric line”输入项。
- 非强制三行显示时，插件会像 MusicPlayer2 本体一样从同时间戳并列歌词中选择一条副行显示。
- `parallel_lyric_line=0` 表示显示末行，适合“原文 / 罗马音 / 翻译”这类歌词，默认通常显示翻译。
- `parallel_lyric_line=1` 到 `3` 表示显示指定副行。
- 强制三行显示仍然保留多行展示，不受单行选择影响。

## 配置示例

```ini
[config]
max_display_lines=3
force_three_lines=0
show_song_info=1
show_current_next=0
parallel_lyric_line=0
primary_font_percent=90
supplement_font_percent=80
min_item_width=120
max_item_width=360
```

## 文件

- `MusicPlayer2Lyric.dll`
- `MusicPlayer2Lyric.ini.example`

## 相关仓库

- 修改版 MusicPlayer2：`https://github.com/ImKani/MusicPlayer2-TaskbarLyric`
- 整合发布仓库：`https://github.com/ImKani/MusicPlayer2TaskbarLyric`

## 版权

Copyright (C) 2026 by ImKani.
