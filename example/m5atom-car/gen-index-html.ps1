Invoke-WebRequest -Uri "https://twosquirrels.github.io/virtual-gamepad/index.html.br" -OutFile "index.html.br"

$bytes = [System.IO.File]::ReadAllBytes("index.html.br")
$hexArray = $bytes | ForEach-Object { "0x{0:x2}" -f $_ }
$hexString = $hexArray -join ", "

$output = @"
#include <stddef.h>

// Brotli-compressed HTML data
const unsigned char index_html_br[] = {
$hexString
};

const size_t index_html_br_len = sizeof(index_html_br);
"@

$output | Out-File -Encoding ASCII -FilePath "index-html.h"

Write-Host "index-html.h has been created successfully with Brotli-compressed data."
