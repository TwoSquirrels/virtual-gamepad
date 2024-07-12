# Virtual Gamepad

マイコンカー操作などのための、スマホ用バーチャルコントローラーです。(もちろん PC も対応しています)

<https://twosquirrels.github.io/virtual-gamepad/> から使用できますが、自分用に作ったので汎用性は低いかもしれません。気に入らない場合は Fork して改造してください。Pull Request 大歓迎です。

> [!TIP]  
> ジョイスティック部分は [nippleJS](https://github.com/yoannmoinet/nipplejs) を使っています。  
> シンプルな構成で作ったので、Web 初心者でも読みやすいかもしれません。コメントが少ないのはごめんなさい。

## License

&copy; 2024 TwoSquirrels  
[MIT License](LICENSE) に従ってください。

## Usage

### サーバー側 (マイコン等)

1. [以下の規格](#Protocol) の形式の HTTP リクエストを処理できるプログラムを走らせます。(TODO: Arduino WiFi.h によるサンプル)
2. 対象のサーバーと通信できるか確認します。

### コントローラー側

3. Web ページを開き、対象サーバー (マイコン等) の URL を host に入力します。
4. ジョイスティックの送信間隔 (デフォルト $125~\mathrm{ms}$) を調整します。
5. A, B ボタンを使わない場合は隠します。
6. 設定画面を閉じ、スワイプまたは WASD (+ Shift) で操作します。

## Protocol

> [!WARNING]  
> このサイトのクローンを対象サーバー上でホスティングする場合などを除き、基本的にレスポンスヘッダーに `Access-Control-Allow-Origin: https://twosquirrels.github.io` 等を加えないとエラーが出て上手く動作しない可能性があります。

### POST `/joystick?p=aaff`

#### Request

ジョイスティックが操作されている時と WASD 操作している時に設定した間隔で、ジョイスティックの極座標が以下の形式で $4$ 桁の $16$ 進数に変換された文字列がクエリに載って送信されます。

- `aa`: 右方向を $0$ として、そこから左回りで一周 $256$ の偏角 $0 \leq \theta < 256$ の整数値を $16$ 進数 $2$ 桁で表した文字列
- `ff`: 中心を $0$、スティックの限界を $255$ とした動径 $0 \leq r < 256$ の整数値を $16$ 進数 $2$ 桁で表した文字列 (WASD 操作の場合は Shift 押下時 `7F` となります)

body は常に空です。

#### Response

デバッグコンソールに表示されることを除けば、無視されます。

### POST `/a`, `/b`

#### Request

画面右側のボタンを押した時に、ボタンの中身に対応したパスに送信されます。body は常に空です。

#### Response

デバッグコンソールに表示されることを除けば、無視されます。

## Tech Stack

**Site Builder:** [Vite](https://ja.vitejs.dev/)  
**Languages:** HTML, JavaScript, CSS  
**Design:** [sanitize.css](https://csstools.github.io/sanitize.css/)  
**Package Manager:** [pnpm](https://pnpm.io/ja/)  
**Hosting:** [GitHub Pages](https://docs.github.com/ja/pages/getting-started-with-github-pages/about-github-pages)
