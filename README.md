# hedgehog

ESP32-C3 SuperMini と HW-627（DRV8833）で、歩行ロボットの左右の足モーターを制御する PlatformIO プロジェクトです。

## 配線

ESP とモーター電源の GND は共通にします。モーター電源は HW-627 の VM / VIN に接続し、ESP の 3V3 や 5V からは取りません。DRV8833 の電源範囲はおおよそ 2.7〜10.8V です。

| ESP32-C3 SuperMini | HW-627 | 役割 |
| --- | --- | --- |
| GPIO4 | IN1（AIN1） | 左足 |
| GPIO5 | IN2（AIN2） | 左足 |
| GPIO6 | IN3（BIN1） | 右足 |
| GPIO7 | IN4（BIN2） | 右足 |
| GND | GND | 共通グラウンド |
| — | OUT1 / OUT2 | 左モーター |
| — | OUT3 / OUT4 | 右モーター |

STBY や nSLEEP 端子がある基板は 3V3 に接続します。

## ビルドと書き込み

```powershell
pio run
pio run -t upload
pio device monitor
```

シリアルモニタは 115200 bps です。

## シリアルコマンド

速度は -255〜255 です。正で正転、負で逆転、0 はそのモーターをブレーキします。範囲外は丸められます。

| コマンド | 動作 |
| --- | --- |
| `L 180` | 左足 |
| `R -180` | 右足 |
| `B 120` | 両足 |
| `S` | ブレーキ停止 |
| `C` | 惰性停止 |
