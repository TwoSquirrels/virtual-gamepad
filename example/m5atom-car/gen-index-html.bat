@echo off
REM Brotli圧縮されたHTMLをダウンロードし、C++バイト配列に変換

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0gen-index-html.ps1"

if %errorlevel% neq 0 (
    echo Error occurred during generation
    exit /b %errorlevel%
)
