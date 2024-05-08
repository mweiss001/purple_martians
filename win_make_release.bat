set version=7.24.0507.1

mkdir purple_martians_%version%_windows
mkdir purple_martians_%version%_windows\screenshots
mkdir purple_martians_%version%_windows\logs
mkdir purple_martians_%version%_windows\data
mkdir purple_martians_%version%_windows\savegame
mkdir purple_martians_%version%_windows\savegame\demo


xcopy pm.exe purple_martians_%version%_windows\  /y /i
xcopy license.txt purple_martians_%version%_windows\  /y /i
#xcopy pm.cfg purple_martians_%version%_windows\  /y /i

xcopy bitmaps purple_martians_%version%_windows\bitmaps  /y /i
xcopy help purple_martians_%version%_windows\help  /y /i
xcopy savegame\demo purple_martians_%version%_windows\savegame\demo /y /i /s
xcopy snd purple_martians_%version%_windows\snd /y /i

xcopy levels\level0* purple_martians_%version%_windows\levels  /y /i
