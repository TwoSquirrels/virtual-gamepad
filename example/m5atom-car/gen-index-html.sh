#!/bin/bash

# --- オプション 1: URLからダウンロード (デフォルト) ---
curl -o index.html.br https://twosquirrels.github.io/virtual-gamepad/index.html.br

# --- オプション 2: ローカルのビルド済みファイルを使用 ---
# cp ../../dist/index.html.br ./index.html.br

# バイナリデータをC++バイト配列に変換
# 注: xxd -i は適切なフォーマットでCスタイルの配列初期化を生成します
{
    echo '#include <stddef.h>'
    echo ''
    echo '// Brotli-compressed HTML data'
    echo 'const unsigned char index_html_br[] = {'
    xxd -i < index.html.br | sed 's/^/  /' | sed '$ s/,$//'
    echo '};'
    echo ''
    echo "const size_t index_html_br_len = sizeof(index_html_br);"
} > index-html.h

echo "index-html.h has been created successfully with Brotli-compressed data."
