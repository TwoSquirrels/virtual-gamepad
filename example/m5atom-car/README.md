# M5Atom WiFi Control Car Example

M5Atom によるマイコンカーでの Virtual Gamepad 用サンプルプログラムです。

## How to RUN

まず[このリポジトリをダウンロード](https://github.com/TwoSquirrels/virtual-gamepad/archive/refs/heads/main.zip)し、この `/example/m5atom-car/` ディレクトリ (フォルダー) を開いてください。

これをコンパイルするためには、`./index-html.h` というファイルを用意してその中で `index_html_br` 定数に <https://twosquirrels.github.io/virtual-gamepad/index.html.br> の Brotli 圧縮された HTML を `const unsigned char[]` 型で設定する必要があります。  
この作業は Windows の場合は `./gen-index-html.bat` を (エクスプローラーの場合はファイルをダブルクリック)、Mac 等 bash が動く環境の場合は `./gen-index-html.sh` を実行することで、**自動で Brotli 圧縮された HTML ファイルをダウンロードしバイト配列としてヘッダファイルを生成できるため、それを使うことを推奨します**。

> [!NOTE]  
> `gen-index-html.sh` を実行するには `xxd` コマンドが必要です。Linux や macOS では通常プリインストールされていますが、もしインストールされていない場合は、以下のコマンドでインストールできます：
> - Ubuntu/Debian: `sudo apt install xxd`
> - macOS (Homebrew): `brew install xxd` または `brew install vim`
> - その他の Linux: `vim-common` パッケージをインストール

また WiFi の ssid と password は `./m5atom-car.ino` に直接書き込んでも良いですが、`./env.h` を作りこのヘッダファイル内で以下のように定数を初期化することもできます。

```c
const char ssid[] = "YOUR_SSID";
const char password[] = "YOUR_PASSWORD";
```

**シリアルモニタを開いた状態で**スケッチを書き込むと、書き込みが完了しプログラムが実行されたタイミングで M5Atom のローカル IP アドレスが表示されるため、それをスマホ等のブラウザで開いてください。ゲームパッドの画面が開けたら成功です。

操作にラグがあるかもしれませんが、**interval の値は表示される ping 値の平均より大きい値を設定する**と少し快適になると思います。ですが多少のラグは諦めてください。

## Structure

`./SimpleHTTPServer.h` は ChatGPT で作った Arduino 用の簡易的な HTTP サーバーライブラリです。  
`./m5atom-car.ino` では Brotli 圧縮された HTML を配信し、ジョイスティックのクエリからパラメータをパースし、`updateMotors` 関数でその値から左右のモーターの出力を設定しています。

> [!TIP]  
> この例ではモーターの出力はかなり雑な計算式で設定しているので操作しにくいと思います。  
> そのため、自分で実験しながら操作しやすい関数を作ることをおすすめします。  
> またモーターの逆回転には対応してないので、対応させたい場合は自分で書いてみましょう。
