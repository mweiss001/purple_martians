cmake_minimum_required(VERSION 4.1)
project(pm)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -static")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic -static")

set(ALLEGRO_ROOT_DIR "C:/allegro")
include_directories(${ALLEGRO_ROOT_DIR}/include)
link_directories(${ALLEGRO_ROOT_DIR}/lib)

add_executable(pm
        src/icon.rc
        src/libnet.h
        src/pm.h
        src/main.cpp
        src/mwBitmap.cpp
        src/mwBitmap.h
        src/mwBitmapTools.cpp
        src/mwBitmapTools.h
        src/mwBottomMessage.cpp
        src/mwBottomMessage.h
        src/mwCodeStats.cpp
        src/mwCodeStats.h
        src/mwColor.cpp
        src/mwColor.h
        src/mwConfig.cpp
        src/mwConfig.h
        src/mwDemoMode.cpp
        src/mwDemoMode.h
        src/mwDemoRecord.cpp
        src/mwDemoRecord.h
        src/mwDisplay.cpp
        src/mwDisplay.h
        src/mwDrawSequence.cpp
        src/mwDrawSequence.h
        src/mwEnemy.cpp
        src/mwEnemy.h
        src/mwEnemyArchwagon.cpp
        src/mwEnemyCannon.cpp
        src/mwEnemyCloner.cpp
        src/mwEnemyEditorFnx.cpp
        src/mwEnemyFlapper.cpp
        src/mwEnemyFnx.cpp
        src/mwEnemyJumpworm.cpp
        src/mwEnemyTrakbot.cpp
        src/mwEnemyVinePod.cpp
        src/mwEventQueue.cpp
        src/mwEventQueue.h
        src/mwFileIterator.cpp
        src/mwFileIterator.h
        src/mwFont.cpp
        src/mwFont.h
        src/mwGameEvent.cpp
        src/mwGameEvent.h
        src/mwGameMoves.cpp
        src/mwGameMoves.h
        src/mwGlobalLevelTool.cpp
        src/mwGlobalLevelTool.h
        src/mwGmInfo.cpp
        src/mwGmInfo.h
        src/mwGraph.cpp
        src/mwGraph.h
        src/mwHelp.cpp
        src/mwHelp.h
        src/mwInput.cpp
        src/mwInput.h
        src/mwItem.cpp
        src/mwItem.h
        src/mwItemBombRocket.cpp
        src/mwItemDoor.cpp
        src/mwItemEditorFnx.cpp
        src/mwItemKeySwitch.cpp
        src/mwItemMessage.cpp
        src/mwItemStartExitGate.cpp
        src/mwItemTrigger.cpp
        src/mwLevel.cpp
        src/mwLevel.h
        src/mwLevelData.cpp
        src/mwLevelDataStats.cpp
        src/mwLift.cpp
        src/mwLift.h
        src/mwLog.cpp
        src/mwLog.h
        src/mwLogGraph.cpp
        src/mwLogo.cpp
        src/mwLogo.h
        src/mwLogViewer.cpp
        src/mwLoop.cpp
        src/mwLoop.h
        src/mwLoopLevelDone.cpp
        src/mwLoopRemoteControl.cpp
        src/mwMain.cpp
        src/mwMain.h
        src/mwMainArgs.cpp
        src/mwMenu.cpp
        src/mwMenu.h
        src/mwMiscFnx.cpp
        src/mwMiscFnx.h
        src/mwMulticolorLine.cpp
        src/mwMulticolorLine.h
        src/mwNetgame.cpp
        src/mwNetgame.h
        src/mwNetgameClient.cpp
        src/mwNetgameServer.cpp
        src/mwNetgameServerFileTransfer.cpp
        src/mwNetgameServerSession.cpp
        src/mwPacketBuffer.cpp
        src/mwPacketBuffer.h
        src/mwPDE.cpp
        src/mwPDE.h
        src/mwPlayer.cpp
        src/mwPlayer.h
        src/mwQuickGraph.cpp
        src/mwQuickGraph.h
        src/mwQuickGraph2.cpp
        src/mwQuickGraph2.h
        src/mwRollingAverage.cpp
        src/mwRollingAverage.h
        src/mwScreen.cpp
        src/mwScreen.h
        src/mwScreenOverlay.cpp
        src/mwScreenOverlayDemoControls.cpp
        src/mwSettings.cpp
        src/mwSettings.h
        src/mwShot.cpp
        src/mwShot.h
        src/mwSolid.cpp
        src/mwSolid.h
        src/mwSound.cpp
        src/mwSound.h
        src/mwSql.cpp
        src/mwSql.h
        src/mwStateHistory.cpp
        src/mwStateHistory.h
        src/mwStatusBuffer.cpp
        src/mwStatusBuffer.h
        src/mwTally.cpp
        src/mwTally.h
        src/mwTriggerEvent.cpp
        src/mwTriggerEvent.h
        src/mwVisualLevel.cpp
        src/mwVisualLevel.h
        src/mwWidget.cpp
        src/mwWidget.h
        src/mwWindow.cpp
        src/mwWindow.h
        src/mwWindowEditorMain.cpp
        src/mwWindowEditSelection.cpp
        src/mwWindowGroupEdit.cpp
        src/mwWindowManager.cpp
        src/mwWindowManager.h
        src/mwWindowObjectViewer.cpp
        src/mwWindowTileHelper.cpp

        src/sqlite3.c
        src/sqlite3.h
)


target_link_libraries(pm
        allegro_monolith-static
        dumb
        freetype
        png16
        zlib
        dsound
        gdiplus
        uuid
        kernel32
        winmm
        psapi
        opengl32
        glu32
        user32
        comdlg32
        gdi32
        shell32
        ole32
        advapi32
        ws2_32
        shlwapi
        jpeg
        webp
        webpdecoder
        FLAC
        theoradec
        vorbisfile
        vorbis
        opusfile
        opus
        ogg
        openmpt
        sharpyuv
        net
        wsock32
)

set_target_properties(pm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
)















