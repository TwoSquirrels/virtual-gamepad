#!/bin/bash

# Download the Brotli-compressed HTML
curl -o index.html.br https://twosquirrels.github.io/virtual-gamepad/index.html.br

# Convert binary data to C++ byte array
# Using od which is more widely available than xxd
{
    echo '#include <stddef.h>'
    echo ''
    echo '// Brotli-compressed HTML data'
    echo 'const unsigned char index_html_br[] = {'
    od -An -tx1 -v < index.html.br | \
        tr -s ' ' | \
        sed 's/^ //' | \
        sed 's/ $//' | \
        sed 's/ /, 0x/g' | \
        sed 's/^/  0x/' | \
        sed 's/$/,/' | \
        sed '$ s/,$//'
    echo '};'
    echo ''
    echo "const size_t index_html_br_len = sizeof(index_html_br);"
} > index-html.h

echo "index-html.h has been created successfully with Brotli-compressed data."
