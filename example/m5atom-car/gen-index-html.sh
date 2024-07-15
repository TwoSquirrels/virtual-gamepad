#!/bin/bash

curl -o index.html https://twosquirrels.github.io/virtual-gamepad/

{
    echo 'const char index_html[] = R"***('
    cat index.html
    echo ')***";'
} > index-html.h

echo "index-html.h has been created successfully."
