# TrafficMonitorPlugins-MusicPlayer2Lyric

这是 `zhongyang219/TrafficMonitorPlugins` 的个人 fork，新增了一个 `MusicPlayer2Lyric` 插件，用于在 TrafficMonitor 任务栏窗口中显示 MusicPlayer2 当前歌词。

这个插件不是独立播放器插件，它需要配合修改版 MusicPlayer2 使用：

```text
https://github.com/ImKani/MusicPlayer2-TaskbarLyric
```

集成说明和发布包在：

```text
https://github.com/ImKani/MusicPlayer2TaskbarLyric
```

上游项目：

```text
https://github.com/zhongyang219/TrafficMonitorPlugins
```

## 这个 fork 改了什么

- 新增 TrafficMonitor 插件工程：

```text
Plugins/MusicPlayer2Lyric
```

- 插件通过共享内存读取修改版 MusicPlayer2 发布的歌词状态。
- 支持显示当前主歌词、并列歌词行、下一句歌词提示。
- 支持同时间戳多行歌词的三行显示，例如：

```text
主歌词
罗马音/发音
翻译
```

- 插件有自己的 ini 设置文件：

```text
MusicPlayer2Lyric.ini
```

## 源码位置

主要文件：

```text
Plugins/MusicPlayer2Lyric/MusicPlayer2Lyric.cpp
Plugins/MusicPlayer2Lyric/MusicPlayer2Lyric.h
Plugins/MusicPlayer2Lyric/MusicPlayer2LyricItem.cpp
Plugins/MusicPlayer2Lyric/MusicPlayer2LyricItem.h
Plugins/MusicPlayer2Lyric/Mp2LyricIpc.h
Plugins/MusicPlayer2Lyric/Mp2LyricIpcReader.cpp
Plugins/MusicPlayer2Lyric/DataManager.cpp
Plugins/MusicPlayer2Lyric/DataManager.h
```

解决方案入口：

```text
TrafficMonitorPlugins.sln
```

## 插件配置

插件会在 TrafficMonitor 提供的插件配置目录中生成：

```ini
[config]
max_display_lines=3
force_three_lines=0
primary_font_percent=90
supplement_font_percent=80
min_item_width=120
max_item_width=360
```

配置说明：

- `max_display_lines`
  - 最多显示行数，范围 `1` 到 `3`。
  - 设为 `3` 时，插件在空间足够时显示“主歌词 + 并列行 1 + 并列行 2”。
- `force_three_lines`
  - `0`：根据任务栏高度自动降级为 2 行或 1 行。
  - `1`：只要当前歌词有两条补充行，就强制三行显示。
  - 如果任务栏高度太小，文字会更挤，依靠省略号截断。
- `primary_font_percent`
  - 三行显示时第 1 行字号比例，范围 `50` 到 `100`。
- `supplement_font_percent`
  - 第 2/3 行字号比例，范围 `50` 到 `100`。
- `min_item_width`
  - 插件项最小宽度。
- `max_item_width`
  - 插件项最大宽度。

如果你想强制任务栏显示三行，可以这样设置：

```ini
[config]
max_display_lines=3
force_three_lines=1
primary_font_percent=90
supplement_font_percent=70
min_item_width=160
max_item_width=420
```

也可以在 TrafficMonitor 的“插件管理”中选中 `MusicPlayer2 歌词` 后点击“选项”修改这些设置。手动修改 ini 后，重启或重载 TrafficMonitor 生效。

## 构建

当前本地使用的构建环境：

- Visual Studio Build Tools
- MSVC v143
- MFC 和 ATL for v143
- Windows SDK `10.0.26100.0`

构建插件：

```powershell
cmd /c "call C:\BuildTools\Common7\Tools\VsDevCmd.bat -arch=x64 -host_arch=x64 >nul && msbuild TrafficMonitorPlugins.sln /t:MusicPlayer2Lyric /p:Configuration=Release /p:Platform=x64 /m"
```

构建输出：

```text
bin\x64\Release\MusicPlayer2Lyric.dll
```

## 许可证

本仓库基于 `zhongyang219/TrafficMonitorPlugins`，遵循上游 MIT License。

注意：这个插件需要配合修改版 MusicPlayer2 使用。MusicPlayer2 上游项目使用 GPLv3，因此如果分发包含修改版 MusicPlayer2 的二进制包，应同时提供对应源码。
