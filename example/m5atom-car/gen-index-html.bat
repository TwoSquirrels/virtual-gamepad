@echo off
REM Download Brotli-compressed HTML and convert to C++ byte array

powershell -NoProfile -ExecutionPolicy Bypass -Command "& {
    # Download the Brotli-compressed HTML
    Invoke-WebRequest -Uri 'https://twosquirrels.github.io/virtual-gamepad/index.html.br' -OutFile 'index.html.br'
    
    # Read file as byte array
    $bytes = [System.IO.File]::ReadAllBytes('index.html.br')
    
    # Create output file with header
    $writer = [System.IO.StreamWriter]::new('index-html.h', $false, [System.Text.Encoding]::ASCII)
    
    try {
        # Write header
        $writer.WriteLine('#include <stddef.h>')
        $writer.WriteLine('')
        $writer.WriteLine('// Brotli-compressed HTML data')
        $writer.WriteLine('const unsigned char index_html_br[] = {')
        
        # Process bytes in chunks to avoid memory issues
        $chunkSize = 12
        for ($i = 0; $i -lt $bytes.Length; $i += $chunkSize) {
            $end = [Math]::Min($i + $chunkSize, $bytes.Length)
            $chunk = $bytes[$i..($end - 1)]
            $hexValues = $chunk | ForEach-Object { '0x{0:x2}' -f $_ }
            
            # Add comma after each value except the last one
            if ($end -lt $bytes.Length) {
                $line = '  ' + ($hexValues -join ', ') + ','
            } else {
                $line = '  ' + ($hexValues -join ', ')
            }
            
            $writer.WriteLine($line)
        }
        
        # Write footer
        $writer.WriteLine('};')
        $writer.WriteLine('')
        $writer.WriteLine('const size_t index_html_br_len = sizeof(index_html_br);')
        
        Write-Host 'index-html.h has been created successfully with Brotli-compressed data.'
    }
    finally {
        $writer.Close()
    }
}"

if %errorlevel% neq 0 (
    echo Error occurred during generation
    exit /b %errorlevel%
)
