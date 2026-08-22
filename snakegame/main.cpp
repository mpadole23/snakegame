#include <iostream>
#include "raylib.h"
#include <deque>
#include <raymath.h>

int cellSize = 30;
int cellCount = 25;
float lastUpdate = 0;

Color green = { 0,158,47,255 };
Color blue = { 102, 191, 255, 255 };
Color red = { 230, 41, 55, 255 };

bool eventTriggered(float interval) {

	float time = GetTime();

	if (time - lastUpdate >= interval) {
		lastUpdate = time;
		return true;
	}
	return false;
}

class Food {
public:
	Vector2 position = getRandomPosition();

	void Draw() {
		Rectangle appleShape = Rectangle{ position.x * cellSize, position.y * cellSize, (float)cellSize, (float)cellSize };
		DrawRectangleRounded(appleShape, 0.8, 8, red);
	}

	Vector2 getRandomPosition() {
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		
		return Vector2{ x,y };
	}

};

class Snake {
private:
	Vector2 direction = { 0,0 };
	Vector2 lastDirection = { 0,0 };

public:

	std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
	

	void Draw() {
		for (unsigned int i = 0; i < body.size(); i++) {
			
			float x = body[i].x;
			float y = body[i].y;
			
			Rectangle segment = Rectangle{ x * cellSize, y * cellSize, (float)cellSize, (float)cellSize };
			DrawRectangleRounded(segment, 0.3, 6, blue);
		}
	}

	void getInput() {

		if (IsKeyPressed(KEY_W) && lastDirection.y != 1) {
			direction = { 0,-1 };
		}
		else if (IsKeyPressed(KEY_A) && lastDirection.x != 1) {
			direction = { -1,0 };
		}
		else if(IsKeyPressed(KEY_S) && lastDirection.y != -1){
			direction = { 0,1 };
		}
		else if (IsKeyPressed(KEY_D) && lastDirection.x != -1) {
			direction = { 1,0 };
		}

	}

	void moveWithoutApple() { //move the snake forward and remove the last segment of the tail (snake does not grow)
		if (direction.x != 0 || direction.y != 0) {
			body.push_front(Vector2Add(body[0], direction));
			body.pop_back();

			lastDirection = direction;
		}
	}
	void moveWithApple() { //move the snake forward and add a new segment to the tail (snake grows)
		if (direction.x != 0 || direction.y != 0) {
			body.push_front(Vector2Add(body[0], direction));

			lastDirection = direction;
		}
	}
};

class Game {
public:
	Food apple;
	Snake snake;

	void draw(){
		apple.Draw();
		snake.Draw();
	}

	bool checkForApple(Vector2& position, std::deque<Vector2>& body) { //check if snake head met apple

		if (body[0].x == position.x && body[0].y == position.y) {
			position = apple.getRandomPosition();
			return true;
		}

		return false;
	}

	void update() {
		if (checkForApple(apple.position, snake.body)) {
			snake.moveWithApple();
		}
		else {
			snake.moveWithoutApple();
		}
	}

	void snakeInput() {
		snake.getInput();
	}

};

int main() {
	std::cout << "Starting game...";
	InitWindow(cellSize*cellCount, cellSize*cellCount, "Snake Game");
	SetTargetFPS(60);

	Game game;


	while (WindowShouldClose() == false) {
		BeginDrawing();
		
		game.snakeInput(); //get direction of snake
		
		if (eventTriggered(0.2)) { //move the snake forward and check if it ate the apple
			game.update();
		}


		ClearBackground(green); //draw the background, snake, apple
		game.draw();

		

		EndDrawing();

	}

	CloseWindow();

}