#ifndef DATATYPE_H
#define DATATYPE_H

using datetype = unsigned int;
using errortype = int;

#define FILE_WEATHER_CITYADCODE_NAME "/opt/citycode_1.txt"
#define FILE_WEATHER_WEATHER_NAME "/opt/cityweather_1.txt"

#define FILE_SCAN_SONG_FROMUSB "/run/media/sda1/myMusic"
#define FILE_SCAN_VIDEO_FROMUSB "/run/media/sda1/myVideo"

#define FILE_SETTIME_NET_NAME "/opt/time_1.txt"



#define WARNING_SCAN_SONGS "请插入U盘并将歌曲放入myMusic目录下"
#define WARNING_SCAN_VIDEOS "请插入U盘并将视频放入myVideo目录下"



// 多媒体视频音量控制
#define MULTIMEDIA_VIDEO_VOLUME_MINIUM 0
#define MULTIMEDIA_VIDEO_VOLUME_MAXIUM 100
#define MULTIMEDIA_VIDEO_VOLUME_DEFAULT 50
#define MULTIMEDIA_VIDEO_VOLUME_CHANGE 10



// 设置模块
#define SETTING_CONFIG_FILENAME "/opt/config/setting.conf"
//#define SETTING_CONFIG_FILENAME "/home/xu/config/setting.conf"

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
//#define ALARM_CLOCK_RING_NAME1 "/home/xu/rings/clock01.mp3"
#define ALARM_CLOCK_RING_NAME1 "/opt/rings/clock01.mp3"

#endif // DATATYPE_H
