
#include<Wire.h>
#include"rgb_lcd.h"
#include "mmap.h"

using namespace std;

/* assume /tmp mounted on /tmpfs -> all operation in memory */
/* we can use just any file in tmpfs. assert(file size not modified && file permissions left readable) */
struct mmapData* p_mmapData; // here our mmapped data will be accessed
rgb_lcd lcd;
const int colorR = 238;
const int colorG = 99;
const int colorB = 99;

int led3 = 2;
boolean flag = true;

typedef struct{
   double temperature;
   double sound;
   double touch;
   double vibration;
  }Sensor_t;


void exitError(const char* errMsg) {
  /* print to the serial Arduino is attached to, i.e. /dev/ttyGS0 */
  string s_cmd("echo 'error: ");
  s_cmd = s_cmd + errMsg + " - exiting' > /dev/ttyGS0";
  system(s_cmd.c_str()); 
  exit(EXIT_FAILURE);
}  

void *playMusic(void *args)
{
  for(int x=0;x<length;x++)//循环音符的次数
  {
    tone(tonePin,tune[x]);//此函数依次播放tune序列里的数组，即每个音符
    delay(400*duration[x]);//每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快，自己掌握吧
    noTone(tonePin);//停止当前音符，进入下一音符
  }
  delay(5000);//等待5秒后，循环重新开始
  
}


void setup() {
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.setCursor(0,0);
  lcd.print("welcome");
  lcd.setCursor(0,1);
  lcd.print("smart home");
  delay(2000);
   lcd.setCursor(0,0);
  lcd.print("sensor value");
  lcd.setCursor(0,1);
  lcd.print("ready to show");
 delay(600);
  int fd_mmapFile; // file descriptor for memory mapped file
  /* open file and mmap mmapData*/
  fd_mmapFile = open(mmapFilePath, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd_mmapFile == -1) exitError("couldn't open mmap file"); 
  /* make the file the right size - exit if this fails*/
  if (ftruncate(fd_mmapFile, sizeof(struct mmapData)) == -1) exitError("couldn' modify mmap file");
  /* memory map the file to the data */
  /* assert(filesize not modified during execution) */
  p_mmapData = static_cast<struct mmapData*>(mmap(NULL, sizeof(struct mmapData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_mmapFile, 0));  
  if (p_mmapData == MAP_FAILED) exitError("couldn't mmap"); 
  /* initialize mutex */
  pthread_mutexattr_t mutexattr; 
  if (pthread_mutexattr_init(&mutexattr) == -1) exitError("pthread_mutexattr_init");
  if (pthread_mutexattr_setrobust(&mutexattr, PTHREAD_MUTEX_ROBUST) == -1) exitError("pthread_mutexattr_setrobust");
  if (pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED) == -1) exitError("pthread_mutexattr_setpshared");
  if (pthread_mutex_init(&(p_mmapData->mutex), &mutexattr) == -1) exitError("pthread_mutex_init");

  /* initialize condition variable */
  pthread_condattr_t condattr;
  if (pthread_condattr_init(&condattr) == -1) exitError("pthread_condattr_init");
  if (pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED) == -1) exitError("pthread_condattr_setpshared");
  if (pthread_cond_init(&(p_mmapData->cond), &condattr) == -1) exitError("pthread_mutex_init");

  /* for this test we just use 2 LEDs */

  pinMode(led3, OUTPUT);


  #ifdef _HULUWA_
  pinMode(tonePin,OUTPUT);//设置蜂鸣器的pin为输出模式
  length = sizeof(tune)/sizeof(tune[0]);//这里用了一个sizeof函数，可以查出tone序列里有多少个音符
  pthread_t tid;
  pthread_create(&tid,NULL,playMusic,NULL);
  pthread_join(tid,NULL);  
  #endif
  
}

void loop() {
  /* block until we are signalled from native code */
   digitalWrite(led3, HIGH);
  if (pthread_mutex_lock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_lock");
  if (pthread_cond_wait(&(p_mmapData->cond), &(p_mmapData->mutex)) != 0) exitError("pthread_cond_wait");
  digitalWrite(led3, LOW);
  double vibration = analogRead(A0);
  double touchValue = analogRead(A1);
  double temperatureValue = analogRead(A2);
  p_mmapData->vibration = vibration;
  p_mmapData->touch = touchValue;
  p_mmapData->soundValue  = analogRead(A3); 
  p_mmapData->temperature =  -0.10581*temperatureValue + 81.6374;
  if(flag == true)
  {
  lcd.setCursor(0,0);  
  lcd.print("vibration:    ");
   lcd.print((int )p_mmapData->vibration);
  lcd.setCursor(0,1);
  lcd.print("pressure:     ");
   lcd.print((int )p_mmapData->touch);
  flag = false;
   }
   else if(flag == false)
   {
    lcd.setCursor(0,0);
  lcd.print("decibel:     ");
   lcd.print((int )p_mmapData->soundValue);
  lcd.setCursor(0,1);
  lcd.print("temperature: ");
   lcd.print((int )p_mmapData->temperature);
  flag = true;
    }
  
  
  
  Serial.println(-0.10581*temperatureValue + 81.6374);
  delay(1000);
  if (pthread_mutex_unlock(&(p_mmapData->mutex)) != 0) exitError("pthread_mutex_unlock");
}
