#define speaker 8 // 喇叭連接在 Pin 8

const int Trig_Pins = 13; // trig 腳位
const int Echo_Pins = 12; // echo 腳位

bool flag = 0;
float distance = 0;

float get_distance(int trig, int echo) {
  float duration;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); //給予trig 10us TTL pulse,讓模組發射聲波
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 30000); // 紀錄echo電位從high到low的時間，就是超音波來回的時間 30ms (約 5 公尺)
  if (duration == 0) return 999; // 若沒讀到距離，視為遠處
  return duration / 29.0 / 2.0; // 聲速340m/s ，換算後約每29微秒走一公分，超音波來回所以再除2
}

void setup() {
  pinMode(Trig_Pins, OUTPUT);
  pinMode(Echo_Pins, INPUT);
  pinMode(speaker, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  distance = get_distance(Trig_Pins, Echo_Pins);
  Serial.print("Distance: "); Serial.print(distance);
  Serial.println(" cm");
  // 根據題目要求的距離區間設定警報聲間隔
  if (distance > 20) {
    // 1. Over 20 cm: Silent
    noTone(speaker); 
    delay(100); 
  } 
  else if (distance > 15 && distance <= 20) playWarning(500); 
  else if (distance > 10 && distance <= 15) playWarning(200);
  else if (distance > 5 && distance <= 10) playWarning(100);
  else if (distance > 0 && distance <= 5) playWarning(50);
  delay(10); // 小幅延遲穩定系統
}

// 輔助函式：發出嗶聲並根據傳入的時間控制間隔
void playWarning(int interval) {
  tone(speaker, 1000); // 發出 1kHz 的聲音
  delay(50);           // 嗶聲持續 50ms
  noTone(speaker);     // 停止發聲
  delay(interval);     // 靜音間隔
}
