# プロジェクトをビルドし、Brotli圧縮されたHTMLをC++バイト配列に変換

$scriptDir = $PSScriptRoot

# --- オプション 1: URLからダウンロード (デフォルト) ---
Write-Host "Downloading index.html.br from URL..."
Invoke-WebRequest -Uri "https://twosquirrels.github.io/virtual-gamepad/index.html.br" -OutFile (Join-Path $scriptDir "index.html.br")

# --- オプション 2: ローカルのビルド済みファイルを使用 ---
# # プロジェクトのルートディレクトリを取得 (スクリプトの場所から2階層上)
# $projectRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
# $sourceFile = Join-Path $projectRoot "dist\index.html.br"
# 
# if (-not (Test-Path $sourceFile)) {
#     Write-Error "File not found: $sourceFile. Please run 'pnpm build' first."
#     exit 1
# }
# 
# Write-Host "Copying index.html.br from dist directory..."
# Copy-Item $sourceFile (Join-Path $scriptDir "index.html.br") -Force

# ファイルが取得できたか確認
if (-not (Test-Path (Join-Path $scriptDir 'index.html.br'))) {
  Write-Error "Failed to obtain index.html.br"
  exit 1
}
# ファイルをバイト配列として読み込み
$bytes = [System.IO.File]::ReadAllBytes((Join-Path $PSScriptRoot 'index.html.br'))

# ヘッダー付きの出力ファイルを作成
$writer = [System.IO.StreamWriter]::new((Join-Path $scriptDir 'index-html.h'), $false, [System.Text.Encoding]::ASCII)

try {
  # ヘッダーを書き込み
  $writer.WriteLine('#include <stddef.h>')
  $writer.WriteLine('')
  $writer.WriteLine('// Brotli-compressed HTML data')
  $writer.WriteLine('const unsigned char index_html_br[] = {')
    
  # メモリの問題を避けるためにバイトをチャンク単位で処理
  # 可読性とシェルスクリプトのフォーマットに合わせるため、1行あたり12バイトを使用
  $chunkSize = 12
  for ($i = 0; $i -lt $bytes.Length; $i += $chunkSize) {
    $end = [Math]::Min($i + $chunkSize, $bytes.Length)
        
    # 配列インデックスを使用してチャンクを抽出
    $chunk = @()
    for ($j = $i; $j -lt $end; $j++) {
      $chunk += $bytes[$j]
    }
        
    $hexValues = $chunk | ForEach-Object { '0x{0:x2}' -f $_ }
        
    # 最後の値以外はカンマを追加
    if ($end -lt $bytes.Length) {
      $line = '  ' + ($hexValues -join ', ') + ','
    }
    else {
      $line = '  ' + ($hexValues -join ', ')
    }
        
    $writer.WriteLine($line)
  }
    
  # フッターを書き込み
  $writer.WriteLine('};')
  $writer.WriteLine('')
  $writer.WriteLine('const size_t index_html_br_len = sizeof(index_html_br);')

  Write-Host 'index-html.h has been created successfully with Brotli-compressed data.'
}
finally {
  $writer.Close()
}