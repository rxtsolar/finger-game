#ifndef _GAME_H_
#define _GAME_H_

#include "../tcp-socket.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace gs {

static const string START = "s";
static const string MOVE = "m";
static const string WAIT = "w";
static const string WON = "y";
static const string LOST = "n";
static const string DATA = "d";
static const string ERROR = "e";

static const string QUIT = "q";
static const string LEFT2LEFT = "ll";
static const string LEFT2RIGHT = "lr";
static const string RIGHT2LEFT = "rl";
static const string RIGHT2RIGHT = "rr";


class Player {
public:
	int left;
	int right;

	Player(void) : left(1), right(1) { }
};

class Game {
public:
	Game(TCPSocket* s) : sock(s), running(true) { }

	void run(void)
	{
		loop();
	}

private:

	void status(void)
	{
		string data;
		sock->recvSocket(data);

		if (data.size() != 4) {
			cerr << "bad data!" << endl;
			return;
		}
		me.left = data[0] - '0' + 1;
		me.right = data[1] - '0' + 1;
		opponent.left = data[2] - '0' + 1;
		opponent.right= data[3] - '0' + 1;

		cout << "               left right" << endl;
		cout << "You:           " << me.left << "    "
			<< me.right << endl;
		cout << "Your opponent: " << opponent.left << "    "
			<< opponent.right << endl;
	}

	void handle(const string& data)
	{
		if (data == START) {
			start();
		} else if (data == MOVE) {
			move();
		} else if (data == WAIT) {
			wait();
		} else if (data == WON) {
			win();
		} else if (data == LOST) {
			lose();
		} else if (data == DATA) {
			status();
		} else if (data == ERROR) {
			error();
		}
	}

	void start(void)
	{
		cout << "Found a worthy opponent!" << endl;
	}

	void move(void)
	{
		string data;

		cout << "Your turn!" << endl;

		do {
			cin >> data;
		} while (!isValid(data));

		sock->sendSocket(data);
	}

	void wait(void)
	{
		cout << "Please wait for your turn." << endl;
	}

	void win(void)
	{
		cout << "Congratulations! You won!" << endl;
		running = false;
	}

	void lose(void)
	{
		cout << "Unfortunately you lost!" << endl;
		running = false;
	}

	void error(void)
	{
		cout << "Invalid move! Try again!" << endl;
		move();
	}

	void loop(void)
	{
		string data;

		cout << "Searching for opponent..." << endl;

		while (running) {
			sock->recvSocket(data);
			handle(data);
		}

		cout << "Good game!" << endl;
	}

	bool isValid(const string& data)
	{
		if (data == LEFT2LEFT &&
				me.left + opponent.left <= 10)
			return true;
		if (data == LEFT2RIGHT &&
				me.left + opponent.right <= 10)
			return true;
		if (data == RIGHT2LEFT &&
				me.right + opponent.left <= 10)
			return true;
		if (data == RIGHT2RIGHT &&
				me.right + opponent.right <= 10)
			return true;
		cout << "local error" << endl;
		return false;
	}

private:
	Player me;
	Player opponent;
	TCPSocket* sock;
	bool running;
};

}

#endif
