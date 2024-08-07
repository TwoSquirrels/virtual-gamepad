# Virtual Gamepad

マイコンカー操作などのための、スマホ用バーチャルコントローラーです。(もちろん PC も対応しています)

<https://twosquirrels.github.io/virtual-gamepad/> から HTML をダウンロードできますが、自分用に作ったので汎用性は低いかもしれません。気に入らない場合は Fork して改造してください。Pull Request 大歓迎です。

> [!TIP]  
> ジョイスティック部分は [nippleJS](https://github.com/yoannmoinet/nipplejs) を使っています。  
> シンプルな構成で作ったので、Web 初心者でも読みやすいかもしれません。コメントが少ないのはごめんなさい。

## License

&copy; 2024 TwoSquirrels  
[MIT License](LICENSE) に従ってください。

## Usage

### サーバー側 (マイコン等)

1. [以下の規格](#Protocol) の形式の HTTP リクエストを処理できるプログラムを走らせます。([参考: M5Atom によるマイコンカーのサンプル](example/m5atom-car/))
2. 対象のサーバーのアドレスを確認します。

### コントローラー側

3. 対象サーバー (マイコン等) のアドレスをブラウザで開きます。
4. ジョイスティックの送信間隔 (デフォルト $100~\mathrm{ms}$) を調整します。通信が詰まらない程度に小さくすることをおすすめします。
5. A, B ボタンを使わない場合は隠します。
6. 設定画面を閉じ、スワイプまたは WASD (+ Shift) で操作します。

## Protocol

> [!WARNING]  
> github.io 上など HTTPS のページから HTTP のホストに対して通信をするとセキュリティエラーが出ます。これはブラウザの設定で対処することもできますが、API と同じホストで HTML を配信してそちらからページにアクセスすることを推奨します。

### GET `/`

#### Request

基本的にブラウザからアクセスされます。

#### Response

ゲームパッドの HTML を返してください。HTML は <https://twosquirrels.github.io/virtual-gamepad/> から最新の物をダウンロードできるようにすることをおすすめします。

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

## Examples

- [M5Atom によるマイコンカーのサンプル](example/m5atom-car/)
- その他のサンプル募集中！

## Tech Stack

**Site Builder:** [Vite](https://ja.vitejs.dev/)  
**Languages:** HTML, JavaScript, CSS  
**Design:** [sanitize.css](https://csstools.github.io/sanitize.css/)  
**Package Manager:** [pnpm](https://pnpm.io/ja/)  
**Hosting:** [GitHub Pages](https://docs.github.com/ja/pages/getting-started-with-github-pages/about-github-pages)
