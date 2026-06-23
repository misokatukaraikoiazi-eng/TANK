# TANK生成プロジェクト

ESP32 と PS4 コントローラー（DualShock 4）を Bluetooth で接続し、入力に応じて LED を制御するプロジェクトです。

## 現在の仕様

### 全体構成

- `app_main()` で Bluetooth 初期化と LED 初期化を行う
- 20ms 周期で PS4 の現在状態を取得し、`led_update()` に渡す
- 入力状態は `ps4_state_t` にまとめて保持する
- 個別 getter も残してあるが、現在は互換用のラッパー
- `motor_control.c` は現時点では未接続のプレースホルダ

### 入力取得

`bluetooth_control.h` では次の API を公開している。

- `init_bluetooth_ps4()`
- `register_paired_ps4_mac(const char *host_mac)`
- `is_paired_ps4_mac_registered(void)`
- `init_bluetooth_ps4_with_host_mac(const char *host_mac)`
- `get_ps4_state(ps4_state_t *out_state)`
- `get_ps4_joystick_values(int8_t *out_x, int8_t *out_y, int8_t *out_rx, int8_t *out_ry)`
- `get_ps4_button_*()` 系の個別 getter

`register_paired_ps4_mac()` は Bluetooth 処理側で受け取った MAC 文字列（`AA:BB:CC:DD:EE:FF` 形式）を登録する。

`is_paired_ps4_mac_registered()` は登録済みMACの有無を返す。

`init_bluetooth_ps4()` は登録済み MAC がある場合のみ ESP32 の Bluetooth MAC 設定を行い、その後に PS4 待受を開始する。未登録時は MAC 設定を変更せず待受だけ開始する。

`init_bluetooth_ps4_with_host_mac()` は、`register_paired_ps4_mac()` と `init_bluetooth_ps4()` をまとめて行う互換 API。

`ps4_state_t` には以下の状態をまとめて格納する。

- 左スティック `lx`, `ly`
- 右スティック `rx`, `ry`
- ボタン類 `cross`, `circle`, `triangle`, `square`, `ps`, `l1`, `r1`, `l2`, `r2`, `share`, `options`, `l3`, `r3`, `up`, `right`, `down`, `left`

### LED 制御

`led_update(const ps4_state_t *state)` は、現在の入力状態を使って 3 種類の出力を行う。

- GPIO25: 左スティック `lx` を PWM 出力に変換
- GPIO26: 右スティック `rx` を PWM 出力に変換
- GPIO27: `circle` で点灯、`triangle` で消灯
- GPIO14: `cross` で点灯、`square` で消灯

スティックの PWM 変換は次のルール。

- デッドゾーンは 15
- 絶対値を `0〜255` に正規化する
- 値は上限 255 にクランプする

### 実行フロー

```c
while (!is_paired_ps4_mac_registered()) {
    const char *mac = get_paired_device_mac();
    if (mac != NULL) {
        register_paired_ps4_mac(mac);
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
}

init_bluetooth_ps4();
led_init();

while (1) {
    get_ps4_state(&state);
    led_update(&state);
    vTaskDelay(pdMS_TO_TICKS(20));
}

```

`main.c` には弱シンボル `get_paired_device_mac()` があり、ペアリング処理モジュール側で同名関数を実装すると差し替えられる。MAC未取得時は1秒間隔で再取得を継続する。

このリポジトリでは `pairing_mac_provider.c` に強い実装を用意してあり、`main.c` の弱い実装を上書きしている。
実運用では `pairing_mac_provider.c` の `get_paired_device_mac()` を、実際のペアリング処理からMACを返す実装に置き換える。

```c
const char *get_paired_device_mac(void) {
    return "AA:BB:CC:DD:EE:FF";
}
```

## ビルド

開発環境では `idf.py build` でビルドできます。

ビルド後は `target_production.bin` が生成されます。

## 補足

- `main.c` のログ出力は現在コメントアウトされている
- `motor_control.c` は今後の拡張用に残している
- 現時点の主用途は「PS4 入力の取得」と「LED の応答確認」
