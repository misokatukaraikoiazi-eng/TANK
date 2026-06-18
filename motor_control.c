#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// ジョイスティックのデッドゾーン（中心付近のブレ）を判定する関数
// 引数: input_x (-128 〜 127), input_y (-128 〜 127)
// 戻り値: デッドゾーン内なら true、範囲外（操作中）なら false を返す
bool is_in_deadzone(int8_t input_x, int8_t input_y) {
    // 【ここにカーソルを置いて改行し、Tabキーを押してください】
    // 1. デッドゾーンのしきい値として、定数「DEADZONE_LIMIT = 15」を定義する
    
    // 2. XとYがそれぞれ中心（0）からどれだけ離れているか絶対値を計算する
    
    // 3. XとYの絶対値が両方とも DEADZONE_LIMIT 以下なら true を返す、そうでなければ false
    #define DEADZONE_LIMIT 15
    
    int8_t abs_x = (input_x < 0) ? -input_x : input_x;
    int8_t abs_y = (input_y < 0) ? -input_y : input_y;

    return (abs_x <= DEADZONE_LIMIT) && (abs_y <= DEADZONE_LIMIT);
}
