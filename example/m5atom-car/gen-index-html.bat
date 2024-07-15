@echo off
setlocal EnableDelayedExpansion

powershell -Command "(New-Object Net.WebClient).DownloadFile('https://twosquirrels.github.io/virtual-gamepad/', 'index.html')"

(
    echo const char index_html[] = R"***(
    type index.html 
    echo )***";
) > index-html.h

echo index-html.h has been created successfully.
