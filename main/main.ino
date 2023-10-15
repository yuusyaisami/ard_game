#include <C:\Users\macar\Documents\Arduino Script\ard_game\ard-game\ard-game.ino>
//ゲーム選択画面
SnakeGame snake_game;
int select_game_index = 0; //snakegame : 0;
Timer select_response_time;
void setup(){
  
}
void loop(){
  if(snake_game.returnselect == false){
    snake_game.handle_event();

    snake_game.update();

    snake_game.draw();

    matrix.clear();
  }
  else {
    input.update();
    if(input.change_input(true) == true){
      if(input.cross_right_key == true){
        select_game_index += 1;
      }
      else if(input.cross_left_key == true){
        if (select_game_index > 0){
         select_game_index -= 1; 
        }
      }
      else if(input.select_up_key == true){
        if(select_game_index == 0){ //スネークゲーム
          snake_game = SnakeGame();
        }
      }
    }
    matrix.map[1][1 + 1 * select_game_index] = 1;
      matrix.draw();
      matrix.clear();
  }
}