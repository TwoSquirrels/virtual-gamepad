# M5Atom WiFi Control Car Example

M5Atom によるマイコンカーでの Virtual Gamepad 用サンプルプログラムです。

## How to RUN

まずこのディレクトリ (フォルダー) をダウンロードしてください。

これをコンパイルするためには、`./index-html.h` というファイルを用意してその中で `index_html` 定数に <https://twosquirrels.github.io/virtual-gamepad/> の HTML を `const char[]` 型で設定する必要があります。  
この作業は Windows の場合は `./gen-index-html.bat` を、Mac 等 bash が動く環境の場合は `./gen-index-html.sh` を実行することで、自動で HTML ファイルをダウンロードしヘッダファイルを生成できるため、それを使うことを推奨します。

また WiFi の ssid と password は `./m5atom-car.ino` に直接書き込んでも良いですが、`./.env.h` を作りこのヘッダファイル内で以下のように定数を初期化することもできます。

```c
const char ssid[] = "YOUR_SSID";
const char password[] = "YOUR_PASSWORD";
```

シリアルモニタを開いた状態でスケッチを書き込むと、書き込みが完了しプログラムが実行されたタイミングで M5Atom のローカル IP アドレスが表示されるため、それをスマホ等のブラウザで開いてください。ゲームパッドの画面が開けたら成功です。

操作にラグがあるかもしれませんが、interval の値は表示される ping 値の平均より大きい値を設定すると少し快適になると思います。ですが多少のラグは諦めてください。

## Structure

`./SimpleHTTPServer.h` は ChatGPT で作った Arduino 用の簡易的な HTTP サーバーライブラリです。  
`./m5atom-car.ino` ではジョイスティックのクエリからパラメータをパースし、`updateMoters` 関数でその値から左右のモーターの出力を設定しています。
