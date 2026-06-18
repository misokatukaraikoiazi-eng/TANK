#!/bin/bash

# 引数（コミットメッセージ）がない場合はエラーを出す
if [ -z "$1" ]; then
    echo "❌ エラー: コミットメッセージを入力してください。"
    echo "使い方: ./push.sh \"ここにメッセージ\""
    exit 1
fi

echo "🔄 Gitの安全宣言を実行中..."
git config --global --add safe.directory /workspaces/TANK

echo "➕ すべての変更をステージング中..."
git add .

echo "📝 コミットを作成中..."
git commit -m "$1"

echo "🚀 GitHubへPush中..."
git push origin main

echo "✅ すべての処理が完了しました！"
