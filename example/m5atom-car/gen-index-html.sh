#!/bin/bash

# Download the Brotli-compressed HTML
curl -o index.html.br https://twosquirrels.github.io/virtual-gamepad/index.html.br

# Convert binary data to C++ byte array
{
    echo '#include <stddef.h>'
    echo ''
    echo '// Brotli-compressed HTML data'
    echo 'const unsigned char index_html_br[] = {'
    hexdump -v -e '16/1 "0x%02x, " "\n"' index.html.br | sed 's/, $//'
    echo '};'
    echo ''
    echo "const size_t index_html_br_len = sizeof(index_html_br);"
} > index-html.h

echo "index-html.h has been created successfully with Brotli-compressed data."
