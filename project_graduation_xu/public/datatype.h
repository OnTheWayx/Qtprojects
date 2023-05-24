#ifndef DATATYPE_H
#define DATATYPE_H

using datetype = unsigned int;
using errortype = int;

// PC or I.MX6ULL
//#define DESKTOP

// 主页天气显示模块
#ifdef DESKTOP
#define FILE_WEATHER_CITYADCODE_NAME "/home/xu/tmp/citycode_1.txt"
#define FILE_WEATHER_WEATHER_NAME "/home/xu/tmp/cityweather_1.txt"
#else
#define FILE_WEATHER_WEATHER_NAME "/opt/tmp/cityweather_1.txt"
#define FILE_WEATHER_CITYADCODE_NAME "/opt/tmp/citycode_1.txt"
#endif

// 多媒体模块
#ifdef DESKTOP
#define FILE_SCAN_SONG_FROMUSB "/home/xu/myMusic"
#define FILE_SCAN_VIDEO_FROMUSB "/home/xu/myVideo"
#else
#define FILE_SCAN_SONG_FROMUSB "/run/media/sda1/myMusic"
#define FILE_SCAN_VIDEO_FROMUSB "/run/media/sda1/myVideo"
#endif

// 自动获取时间
#ifdef DESKTOP
#define FILE_SETTIME_NET_NAME "/home/xu/tmp/time_1.txt"
#else
#define FILE_SETTIME_NET_NAME "/opt/tmp/time_1.txt"
#endif

// 天气预报模块
#ifdef DESKTOP
#define FILE_WEATHER_FORECAST_ADCODE_NAME "/home/xu/tmp/weatherforecastadcode_1.txt"
#define FILE_WEATHER_FORECAST_WEATHER_NAME "/home/xu/tmp/weatherforecastweather_1.txt"
#else
#define FILE_WEATHER_FORECAST_ADCODE_NAME "/opt/tmp/weatherforecastadcode_1.txt"
#define FILE_WEATHER_FORECAST_WEATHER_NAME "/opt/tmp/weatherforecastweather_1.txt"
#endif

#define WARNING_SCAN_SONGS "请插入U盘并将歌曲放入myMusic目录下"
#define WARNING_SCAN_VIDEOS "请插入U盘并将视频放入myVideo目录下"



// 多媒体视频音量控制
#define MULTIMEDIA_VIDEO_VOLUME_MINIUM 0
#define MULTIMEDIA_VIDEO_VOLUME_MAXIUM 100
#define MULTIMEDIA_VIDEO_VOLUME_DEFAULT 50
#define MULTIMEDIA_VIDEO_VOLUME_CHANGE 10



// 设置模块
#ifdef DESKTOP
#define SETTING_CONFIG_FILENAME "/home/xu/config/setting.conf"
#else
#define SETTING_CONFIG_FILENAME "/opt/config/setting.conf"
#endif

// general模块
#define SETTING_GENERAL "setting_general"
// 时间设置
// 是否自动联网校准 true/false
#define SETTING_TIME_ISUPDATEBYNET "setting_time_isupdatebynet"
// 时间偏移至北京时间
#define SETTING_TIME_UPDATE_PRESET 8 * 60 * 60



// alarm模块
#define ALARM_CLOCK "alarm_clock"

// 闹钟
#define ALARM_CLOCK_TIME "alarm_clock_time"

#define ALARM_CLOCK_RING_VOLUEM_DEFAULT 80
// 闹钟铃声
#ifdef DESKTOP
#define ALARM_CLOCK_RING_NAME1 "/home/xu/rings/clock01.mp3"
#else
#define ALARM_CLOCK_RING_NAME1 "/opt/rings/clock01.mp3"
#endif



// 天气预报模块
#define WEATHER_FORECAST "weather_forecast"








#endif // DATATYPE_H
