unsigned char O_SER = 5;
unsigned char O_RCLK = 6;
unsigned char O_SRCLK = 7;

unsigned int C[3] = {130, 262, 523};
  unsigned int Db[3] = {139, 277, 554};
  unsigned int D[3] = {147, 294, 587};
  unsigned int Eb[3] = {155, 331, 622};
  unsigned int E[3] = {165, 330, 659};
  unsigned int F[3] = {175, 349, 698};
  unsigned int Gb[3] = {185, 370, 392};
  unsigned int G[3] = {196, 392, 784};
  unsigned int Ab[3] = {208, 415, 831};
  unsigned int A[3] = {220, 440, 880};
  unsigned int Bb[3] = {233, 466, 932};
  unsigned int B[3] = {247, 494, 988};

unsigned char ColumnPin[8] = {4, 10, 11, 1, 13, 2, 6, 7};
unsigned char RowPin[8] =       {0, 5, 15, 3, 8, 14, 9, 12};
//入力状態を取得するときに使用するクラス
class Input{
  public:
    bool InputData[8] = {false, false, false, false, false, false, false, false,};
    bool InputData_old[8] = {false, false, false, false, false, false, false, false,};
    unsigned int IN_PUT_PIN[8] = {4, 3, 11, 2, 12, 9, 10, 8};
    bool cross_up_key = false;
    bool cross_left_key = false;
    bool cross_right_key = false;
    bool cross_down_key = false;

    bool side_left_key = false;
    bool side_right_key = false;

    bool select_up_key = false;
    bool select_down_key = false;

    bool side_key_ = false;
    bool change = false;
    Input(){
      for (unsigned int i=0; i<8; i++){
        pinMode( IN_PUT_PIN[8] , INPUT );
      }
    }
    //入力状態を取得する。
    void update(){
      for (unsigned int i=0; i<8; i++){
        InputData[i] = (digitalRead(IN_PUT_PIN[i]));
      }
      side_left_key = InputData[0];
      cross_up_key = InputData[1];
      cross_right_key = InputData[2];
      cross_left_key = InputData[3];
      cross_down_key = InputData[4];
      select_up_key = InputData[5];
      select_down_key = InputData[6];
      side_right_key = InputData[7];
    }
    // 入力状態に変化があった場合Trueを返す
    bool change_input(bool not_return_all_zero = false){
    for (unsigned int i=0; i<8; i++){
      if (InputData_old[i] != InputData[i]){
        for (unsigned int j=0; j<8; j++){
          InputData_old[j] = InputData[j];
        }
        if(not_return_all_zero == true){
          for (unsigned int j=0; j<8; j++){
            if(InputData_old[j] == true){
              return true;
            }
          }
          return false;
        }
        return true;
      }
    }
    return false;
}
};
//製作者用
class ShiftRegister {
public:
    bool light_map[8][8];
    bool light_line[64];
    ShiftRegister(){
      
      init();
    }
    void init(){
      for(int i = 0; i < sizeof(light_map) / sizeof(light_map[0]); i++){
        for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){
          light_map[i][j] = false;
        }
      }
      digitalWrite(O_RCLK, LOW);
      for(int i = 0; i < 64;i++){
        digitalWrite(O_SER, LOW);
        //クロックパルスを立ち上げる
    	  digitalWrite(O_SRCLK, HIGH);
    	  digitalWrite(O_SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(O_RCLK, HIGH);
    }
    void get_map(int shift_num , unsigned char map[][16]){
      if (shift_num == 0){
        for (int i = 0; i < 8 ; i++){
          for (int j = 0; j < 8; j++){
            if(map[i][j] != 0){
              light_map[i][j] = true;
            }
            else {
              light_map[i][j] = false;
            }
          }
        }
      }
      if (shift_num == 1){
        for (int i = 0; i < 8 ; i++){
          for (int j = 8; j < 16; j++){
            if(map[i][j] != 0){
              light_map[i][j - 8] = true;
            }
            else {
              light_map[i][j - 8] = false;
            }
          }
        }
      }
      if (shift_num == 3){
        for (int i = 8; i < 16 ; i++){
          for (int j = 0; j < 8; j++){
            if(map[i][j] != 0){
              light_map[i - 8][j] = true;
            }
            else {
              light_map[i - 8][j] = false;
            }
          }
        }
      }
      if (shift_num == 2){
        for (int i = 8; i < 16 ; i++){
          for (int j = 8; j < 16; j++){
            if(map[i][j] != 0){
              light_map[i - 8][j - 8] = true;
            }
            else {
              light_map[i - 8][j - 8] = false;
            }
          }
        }
      }
    }
    void get_line(int raw , unsigned char map[][16]){
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(RowPin) / sizeof(RowPin[0]); i++){
          light_line[RowPin[i] + 16 * shift_num] = true;
        }
      }
      for (int shift_num = 0; shift_num < 4; shift_num++){
        for (int i = 0; i < sizeof(ColumnPin) / sizeof(ColumnPin[0]); i++){
          light_line[ColumnPin[i] + 16 * shift_num] = false;
        }
      }

      for(int r = 0; r < 4; r++){
        get_map(r, map);
        for (int j = 0; j < sizeof(light_map[0]) / sizeof(light_map[0][0]); j++){
          if ( light_map[raw][j] == true){
            light_line[ColumnPin[j] + 16 * r] = true;
            light_line[RowPin[raw] + 16 * r] = false;
          }
        }
        
      }
    }
    void LightUp(unsigned char map[][16]){
      for (int r = 0; r < 8; r++){
      get_line(r, map);
      digitalWrite(O_RCLK, LOW);
      for(int c = 63; c != -1;c--){
        if (light_line[c] == true){
          digitalWrite(O_SER, HIGH);
        }
        else{
          digitalWrite(O_SER, LOW);
        }
        //クロックパルスを立ち上げる
        digitalWrite(O_SRCLK, HIGH);
        digitalWrite(O_SRCLK, LOW);
        //-----------------------------------------
      }
      digitalWrite(O_RCLK, HIGH);
    }
  }
};
class DotMatrix {
public:
    unsigned char map[16][16];
    ShiftRegister shift_register;
    int count = 0;
    DotMatrix(){
      pinMode( O_SER,  OUTPUT );
      pinMode( O_RCLK, OUTPUT );
      pinMode( O_SRCLK, OUTPUT );
      map_init();
      Serial.begin(9600);
    }
    void update(){
    }
    void draw(){
      count += 1;
      if (count % 5 == 0){
        for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
          for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
            if(map[i][j] == 2){
              map[i][j] = 0;
            }
          }
        }
      }
      shift_register.LightUp(map);
    }
    void clear(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          map[i][j] = 0;
        }
      }
    }
    void map_init(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          map[i][j] = 0;
        }
      }
    }
    void map_debug_view(){
      for(unsigned char i = 0; i < sizeof(map) / sizeof(map[0]); i++){
        for (unsigned char j = 0; j < sizeof(map[0]) / sizeof(map[0][0]); j++){
          if (map[i][j] == true){
            Serial.write("1, ");
          }
          else if (map[i][j] == false){
            Serial.write("0, ");
          }
          else{
            Serial.write("N, ");
          }
        }
        Serial.println();
      }
    }
};
class Sound{
  public:
  int pin = 0;
    Sound(int sound_pin){
      pin = sound_pin;
    }
    void Play(unsigned int musical_scale, unsigned int soundtime){
      tone(pin, musical_scale,soundtime);
    }
    void MusicPlay(){

    }
};
Input input;
DotMatrix matrix;
Sound Sound1(A7);
Sound Sound2(A6);
class Timer{
  public:
  int count = 0;
  int goal = 0;
  bool timervisible = true;
  bool onetime;
  int CycleTime = 0;
  Timer(bool one_time = false){
    //数値はゼロから始まり、goal_countで終了
    onetime = one_time;
  }
  bool Do(int goal_count){
    if(timervisible == true){
      count += 1;
    }
    if (count % goal_count == 0){
      if(onetime == true){
        timervisible = false;
      }
      CycleTime += 1;
      return true;
    }
    return false;
  }
};
class Tail{
      public:
      unsigned char x;
      unsigned char y;
      bool visible = false;
    };
    class Food{
    public:
    unsigned char x;
    unsigned char y;
    bool visible = false;
  };
//game classes
class Snake{
  public:
  String direction = "right"; //進行方向
  int speed = 5;//数値が小さいほど遅い
  int x; //スネーク自身の座標 
  int y;
  unsigned char random_x, random_y;
  unsigned int tail_len = 0;
  Tail tail_array[256];
  bool visible = true; //自身を描写するか
  bool gameover = false; //ゲーム終了
  Food food;
  Timer death;
  Timer move;

  Snake(){
    Serial.begin(9600);
    for(unsigned int i = 0; i < 256; i++){
      tail_array[i].visible = false;
    }
    int rx, ry;
    create_random();
    food.x = random_x;
    food.y = random_y;
  }
  void handle_event(){
    if(input.cross_up_key == true){
      direction = "up";
    }
    else if(input.cross_right_key == true){
      direction = "right";
    }
    else if(input.cross_left_key == true){
      direction = "left";
    }
    else if(input.cross_down_key == true){
      direction = "down";
    }
  }
  void update(){
    if(visible == true){
      if(move.Do(speed) == true){
        if(direction == "up"){
          add_tail(x, y); //移動前の座標を挿入する
          y = y - 1;
          if (y < 0){
            y = 15; //マップ外だった場合、そこから対称の地点に移動する
          }
        }
        if(direction == "right"){
          add_tail(x, y);
          x = x + 1;
          if (x > 15){
            x = 0;
          }
        }
        if (direction == "left"){
          add_tail(x, y);
          x = x - 1;
          if (x < 0){
            x = 15;
          }
        }
        if (direction == "down"){
          add_tail(x, y);
          y = y + 1;
          if (y > 15){
            y = 0;
          }
        }
        if(hit_tail() == true){
          visible = false;
        }
        hit_food();
      }
    }
    else{
      if(death.Do(2) == true){
        if (tail_array[death.CycleTime].visible == true){
          tail_array[death.CycleTime].visible = false;

          Sound1.Play(C[1], 50);
          Sound2.Play(B[0], 50);
        }
        else {
          Sound1.Play(A[0], 50);
          gameover = true;
        }
      }
    }
  }
  bool hit_food(){
    if (x == food.x && y == food.y){
      for(unsigned int i = 0; i < 256; i++){
        if(tail_array[i].visible == false){
          tail_array[i].visible = true;
          break;
        }
      }
      Sound1.Play(E[1], 50);
      //Sound2.Play(F[1], 50);
      create_random();
      food.x = random_x;
      food.y = random_y;
        return true;
      }
    return false;
  }
  void add_tail(unsigned int arg_x, unsigned int arg_y){
    array_shift();
    tail_array[0].x = arg_x;
    tail_array[0].y = arg_y;
  }
  void draw(){
    if(visible == true){
       matrix.map[y][x] = 1;
    }
    matrix.map[food.y][food.x] = 2;
    
    for(unsigned int i = 0; i < 256; i++){
      if(tail_array[i].visible == true){
        matrix.map[tail_array[i].y][tail_array[i].x] = 1;
      }
    }

  }
  void create_random(){
    bool cant_break = false;
    while(true){
      int r_x = random(0, 16);
      int r_y = random(0, 16);
      if(r_x == x && r_y == y){
        cant_break = true;
      }
      for(unsigned int i = 0; i < 256; i++){
        if(tail_array[i].visible == true){
          if(r_x == tail_array[i].x && r_y == tail_array[i].y){
            cant_break = true;
          }
        }
      }
      if(cant_break == false){
        random_x = r_x;
        random_y = r_y;
        break;
      }
      else{
        cant_break = false;
      }
    }
  }
  bool hit_tail(){
    for(unsigned int i = 0; i < 256; i++){
      if(tail_array[i].visible == true){
        if(x == tail_array[i].x && y == tail_array[i].y){
          return true;
        }
      }
    }
    return false;
  }
  void array_shift(){
    for(int i = (sizeof(tail_array) / sizeof(tail_array[0]) - 1); i != 0 ; i--){
      tail_array[i].x = tail_array[i - 1].x;
      tail_array[i].y = tail_array[i - 1].y;
    }
  }
};
class SnakeGame {
  public:
    Snake snake;
    bool returnselect = false;
    SnakeGame(){

    }
    void handle_event(){
      input.update();
      if(input.side_right_key == true){
        snake = Snake();
      }
      if(input.side_left_key == true){
        returnselect = true;
      }
      if(snake.gameover == false){
        snake.handle_event();
      }
    }
    void update(){
      if(snake.gameover == false){
        snake.update();
      }
    }
    void draw(){
      snake.draw();
      matrix.draw();
    }
};

class FroggerGame{
  public:
    
};
class Frogger{
  public:
  
};
/*
DotMatrix dot;
Input input;
Sound sound(A1);
*/
//////////////////////////////