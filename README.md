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
- `get_ps4_state(ps4_state_t *out_state)`
- `get_ps4_joystick_values(int8_t *out_x, int8_t *out_y, int8_t *out_rx, int8_t *out_ry)`
- `get_ps4_button_*()` 系の個別 getter

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
init_bluetooth_ps4();
led_init();

while (1) {
    get_ps4_state(&state);
    led_update(&state);
    vTaskDelay(pdMS_TO_TICKS(20));
}
```

## ビルド

開発環境では `idf.py build` でビルドできます。

ビルド後は `target_production.bin` が生成されます。

## 補足

- `main.c` のログ出力は現在コメントアウトされている
- `motor_control.c` は今後の拡張用に残している
- 現時点の主用途は「PS4 入力の取得」と「LED の応答確認」
