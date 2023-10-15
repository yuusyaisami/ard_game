#define SER		(2)
#define RCLK	(3)
#define SRCLK	(4)
class ShiftRegister {
public:
    bool light_map[8] = {false, false, false, false, false, false, false, false};
    ShiftRegister(){
    }
    void init(){
      digitalWrite(RCLK, LOW);
      for(int i = 0; i < 8;i++){
        digitalWrite(SER, LOW);
        //クロックパルスを立ち上げる
    	  digitalWrite(SRCLK, HIGH);
    	  digitalWrite(SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(RCLK, HIGH);
    }
    void LightUp(){
      digitalWrite(RCLK, LOW);
      for(int i = 0; i < 8;i++){
        if (light_map[i] == true){
          digitalWrite(SER, HIGH);
        }
        else{
          digitalWrite(SER, LOW);
        }
        //クロックパルスを立ち上げる
    	  digitalWrite(SRCLK, HIGH);
    	  digitalWrite(SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(RCLK, HIGH);
    }
};
class DotMatrix {
public:
    bool map[16][16];
    ShiftRegister shift_register_L_U;
    ShiftRegister shift_register_L_D;
    ShiftRegister shift_register_R_U;
    ShiftRegister shift_register_R_D;
    DotMatrix(){
    
    }
    void print(){
        Serial.begin(9600);
    }
  
    void map_init(){
      for(int i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (int j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          map[i][j] = false;
        }
      }
    }
};

DotMatrix obj(12);
void setup(){
  Serial.begin(9600);
  obj.print();
}
void loop(){

}