# TANK

# 1. 必要なツール類のアップデート
sudo apt-get update && sudo apt-get install -y wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

# 2. ESP-IDFのソースコードをクローン（今回は安定版の v5.1 を指定）
git clone -b v5.1 --recursive https://github.com/espressif/esp-idf.git ~/esp-idf

# 3. ツールチェーンのインストール
cd ~/esp-idf
./install.sh esp32

cd /workspaces/* # プロジェクトのフォルダに戻る
. ~/esp-idf/export.sh

idf.py build


# 1. 仮想環境のPythonに setuptools を強制インストール
/home/codespace/.espressif/python_env/idf5.1_py3.12_env/bin/python -m pip install -U pip setuptools

# 2. もう一度環境変数を読み直す
. ~/esp-idf/export.sh

# 3. いざ、ビルドを実行！
idf.py build

# 1. TANK（プロジェクトのルート）フォルダにいることを確認
cd /workspaces/TANK

# 2. gccで2つのCファイルをまとめて擬似コンパイル
gcc -fsyntax-only main/main.c main/bluetooth_control.c

# 1. ターミナル起動時に自動でexport.shが走るように設定を追加
echo ". /opt/esp/idf/export.sh" >> ~/.bashrc

# 2. 今のターミナルにそれを即時反映
source ~/.bashrc

esptool.py --chip esp32 merge_bin -o target_production.bin @build/flash_args

esptool.py --chip esp32 merge_bin -o target_production.bin --work-dir build @build/flash_args

# 1. build ディレクトリの中に直接移動
cd /workspaces/TANK/build

# 2. その場で結合を実行（完成品は1つ上のルートに出力）
esptool.py --chip esp32 merge_bin -o ../target_production.bin @flash_args
