if WinActive("ahk_class SunAwtFrame") {
    Send "^s"
    Sleep(200)
    if(WinExist("ahk_class SunAwtDialog")) {
        Send "{Enter}"
    }
    Sleep(1000)
    if(WinExist("ahk_class SunAwtDialog")) {
        WinWaitNotActive
    }
}

Esc::ExitApp
ExitApp