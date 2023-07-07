mkdir purple_martians__windows
mkdir purple_martians__windows\screenshots
mkdir purple_martians__windows\logs
mkdir purple_martians__windows\data
mkdir purple_martians__windows\savegame
mkdir purple_martians__windows\savegame\demo

xcopy data\level_icons_100.bmp purple_martians__windows\data  /y /i
xcopy data\level_icons_200.bmp purple_martians__windows\data  /y /i


xcopy pm.exe purple_martians__windows\  /y /i
xcopy license.txt purple_martians__windows\  /y /i
#xcopy pm.cfg purple_martians__windows\  /y /i

xcopy bitmaps purple_martians__windows\bitmaps  /y /i
xcopy help purple_martians__windows\help  /y /i
xcopy savegame\demo purple_martians__windows\savegame\demo /y /i
xcopy snd purple_martians__windows\snd /y /i

xcopy levels\level0* purple_martians__windows\levels  /y /i



