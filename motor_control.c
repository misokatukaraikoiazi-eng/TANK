// 左スティック(lx, ly)から左右のモーター出力を計算する関数
// デッドゾーンは15、計算結果は-128〜127にクランプする
void calculate_motor_output(int8_t lx, int8_t ly) {
    int8_t left_motor = 0;
    int8_t right_motor = 0;

    // デッドゾーンの設定
    if (abs(lx) < 15) lx = 0;
    
    if (abs(ly) < 15) ly = 0;

    // 左スティックの値を元にモーター出力を計算
    left_motor = ly + lx;   // 前進/後退 + 左右
    right_motor = ly - lx;  // 前進/後退 - 左右

    // 出力を-128〜127にクランプ
    if (left_motor > 127) left_motor = 127;
    if (left_motor < -128) left_motor = -128;
    if (right_motor > 127) right_motor = 127;
    if (right_motor < -128) right_motor = -128;

    // モーター出力を使用してモーターを制御するコードをここに追加
}