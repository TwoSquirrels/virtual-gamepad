Invoke-WebRequest -Uri "https://twosquirrels.github.io/virtual-gamepad/" -OutFile "index.html"

$content = Get-Content -Raw -Path "index.html"
$output = @"
const char index_html[] = R"***(
$content
)***";
"@
$output | Out-File -Encoding ASCII -FilePath "index-html.h"

Write-Host "index-html.h has been created successfully."
