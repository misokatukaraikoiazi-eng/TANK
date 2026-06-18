
# TANK生成プロジェクト (AI utilization play project)
ESP32をベースに、PS4コントローラー（DualShock 4）を使ってBluetooth無線でタンク（戦車ロボ）を制御するプロジェクトです。
本プロジェクトでは、「上流設計：Gemini」、「下流実装・環境構築：GitHub Copilot」というAIの役割分担（プロセス分離）を徹底し、次世代の高速開発スタイルを実践しています。
## 🚀 開発環境・使用デバイス
 * マイコンボード: ESP32
 * コントローラー: PS4 Controller (DualShock 4)
 * 開発環境: GitHub Codespaces (コンテナ環境) / ESP-IDF
 * AIツール: Gemini (無料版), GitHub Copilot
## 🛠️ 最大の難所と突破口：ビルド・フラッシュ環境の自動化
### 【苦労した点】手動による環境構築の罠
クラウド開発環境（GitHub Codespaces）のコンテナから、ローカルの実機へバイナリを焼き込む（フラッシュ）ためのパス通し、環境変数設定、CMakeのポストビルド拡張は非常に複雑でした。手動で泥臭く設定しようとすると、タイポひとつでビルドエラーの沼にハマり、多くの時間を溶かす原因になります。実際、手動での接続テスト段階では Connecting........_ で止まるなど、シリアルポート制御や環境の紐付けで大きな壁にぶつかりました。
### 【解決策】GitHub Copilotへの丸投げ
「ややこしい設定は、多少トークンを消費してでもAIに任せるべき」という方針に切り替え、GitHub Copilotにビルド環境の構築を全面委託しました。結果として、手動では到達困難だった「ビルドから実機焼き込みまでの完全自動化」を一撃でクリアしました。
現在、root/CMakeLists.txt の末尾に以下のカスタムコマンドを組み込み、ターミナルで idf.py build を1行叩くだけで、バイナリの統合（merge_bin）から /dev/ttyUSB0 への書き込みまでがフルオートで走る環境が爆誕しています。
```cmake
# ビルド完了後に自動で特定のポートへ書き込み（フラッシュ）まで連動する設定
add_custom_command(
    TARGET app
    POST_BUILD
    # 1. まずは1本化バイナリ（target_production.bin）を生成
    COMMAND esptool.py --chip esp32 merge_bin -o ${CMAKE_SOURCE_DIR}/target_production.bin @flash_args
    # 2. ポートを指定して一発書き込み（-p で接続先ポートを明示）
    COMMAND esptool.py --chip esp32 -p /dev/ttyUSB0 -b 460800 --before default_reset --after hard_reset write_flash 0x0 ${CMAKE_SOURCE_DIR}/target_production.bin
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating single binary and FLASHING automatically..."
)

```
## 📦 実装された主要アーキテクチャ
### 1. 型安全と徹底したカプセル化（Gemini設計）
将来的なコードのスパゲッティ化（グローバル変数の乱用など）を防ぐため、ps4.h が提供する公式の型（ps4_event_callback_t）に完全準拠した設計を行いました。ボタンやスティックの状態はファイル内に隠蔽し、必ずゲッター関数経由で安全に取得するインターフェースを確立しています。
```c
// bluetooth_control.c (内部で状態を隠蔽)
static bool s_cross_button_pressed = false;

void ps4_packet_callback(ps4_t ps4, ps4_event_t event) {
    s_cross_button_pressed = event.status.button.cross; 
}

// 外部公開用のインターフェース関数
bool get_ps4_button_cross(void) { return s_cross_button_pressed; }

```
### 2. 視認性を極めた「5項目改行」デバッグログ
全ボタンの状態を1行に表示すると横スクロールが発生し、デバッグの効率が著しく低下します。これを解決するため、系統（STICKS, ACTION, BUMPERなど）ごとに5項目目安で改行を入れ、フォーマット指定子（%-3s）で縦ラインが絶対にズレない美しいログ出力を実装しました。
```c
// main.c (200ms周期タスク内)
ESP_LOGI(TAG, "\n"
              "  [STICKS]  LX: %4d | LY: %4d\n"
              "  [ACTION]  Cross: %-3s | Circle: %-3s | Triangle: %-3s | Square: %-3s | PS: %-3s\n"
              "  [BUMPER]  L1:    %-3s | R1:     %-3s | L2:       %-3s | R2:     %-3s",
         x, y, cross ? "ON" : "off", circle ? "ON" : "off", triangle ? "ON" : "off", square ? "ON" : "off", ps ? "ON" : "off",
         l1 ? "ON" : "off",    r1 ? "ON" : "off",     l2 ? "ON" : "off",       r2 ? "ON" : "off");

```
### 3. コメントからの「ドバーっ」と自動補完ロジック
左スティック（lx, ly）の座標からタンクの左右モーターの出力を叩き出す「加減算ミキシングロジック」を実装。エディタ上で日本語で意図を数行書くだけで、GitHub Copilotが空気を読み、デッドゾーン（不感帯15）の処理や型安全なクランプ処理（-128〜127）を含んだ関数を一瞬で自動生成（補完）しました。
#### 駆動ミキシングの基本数式：
 * 左モーター出力 = ly + lx
 * 右モーター出力 = ly - lx
 * 斜めにスティックを倒した際（例: Y=100, X=50）にも、出力が自動的に「左:127, 右:50」となり、狙い通り「約 2:1」の理想的な旋回比率が維持される仕様となっています。
## 💡 実際のAIへの指示（プロンプト・コメント例）
本プロジェクトで実際に高い効果を発揮した、AIへの具体的な指示の出し方（再現用データ）です。
#### ① エディタ上での「ドバーっと自動補完」を誘発したコメント
関数をゼロから書くのではなく、ヘッダー直下に以下の「意図（仕様）」を2行コメントしただけで、Copilotが関数の中身を完全生成しました。
```c
// 左スティック(lx, ly)から左右のモーター出力を計算する関数
// デッドゾーンは15、計算結果は-128〜127にクランプする
void calculate_motor_output(int8_t lx, int8_t ly) {
    // 💡 ここでEnterを押すだけで、必要なすべてのロジックがCopilotにより自動補完された

```
#### ② 複雑なCMake環境構築時にCopilot Chatへ投げたプロンプト
手動設定を諦め、多少のトークン消費を割り切ってCopilotに丸投げした際の指示文です。
プロンプト例：
「ESP-IDF環境において、idf.py build が成功した後に自動的に esptool.py merge_bin を実行して単一バイナリを作成し、さらにそれを指定したシリアルポート（/dev/ttyUSB0）へ自動でフラッシュ（ write_flash 0x0 ）まで連動させるための CMakeLists.txt のカスタムコマンド（add_custom_command）を書いてください。」
この一振りのプロンプトにより、泥臭く環境変数やパスの競合で沼ることなく、一撃でフルオートのビルム環境が手に入りました。
## 💡 AI駆動開発における知見（まとめ）
## 25年の組み込み開発経験に基づく「正しい設計思想の骨組み」を人間（およびGemini）がしっかりと定義し、泥臭く複雑な環境構築やタイピングは下流のGitHub Copilotに任せる。
この「上流の意思決定に人間が集中し、下流の実装はAIにレバレッジをかける」スタイルこそが、組み込み開発における理想的なPDCAサイクルであることを本プロジェクトで実証しました。
