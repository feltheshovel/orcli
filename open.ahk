; SunAwtFrame is the ahk window class of OR
; SunAwtDialog is the ahk window class of OR's "open file" dialog

ORPATH := '"C:\Program Files\OpenRocket\OpenRocket.exe" '

if WinExist("ahk_class SunAwtFrame") {
    WinActivate("ahk_class SunAwtFrame")
    winid := WinGetID("A")
    Send "^o"
    WinWait("ahk_class SunAwtDialog")
    SendText A_Args[1]
    Send "{Enter}"
    WinWaitNotActive(winid)
    if WinActive("ahk_class SunAwtDialog") {
        WinWaitNotActive
    }
    WinWaitNotActive(winid)
    WinClose(winid)
} else  {
    Run(ORPATH . A_Args[1])
    WinWait("ahk_class SunAwtFrame")
}

Esc::ExitApp
ExitApp