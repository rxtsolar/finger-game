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
	TCPSocket* sock;

	Player(TCPSocket* s) : left(1), right(1), sock(s) { }

	void sendtoClient(const string& message)
	{
		sock->sendSocket(message);
	}

	void recvfromClient(string& message)
	{
		sock->recvSocket(message);
	}
};

class Game {
public:
	Game(TCPSocket* sock0, TCPSocket* sock1) :
		player0(sock0), player1(sock1) { }

	void run(void)
	{
		start();
		status();
		loop();
		end();
	}

private:

	void status(void)
	{
		string s0 = to_string(player0.left - 1) +
			to_string(player0.right - 1);
		string s1 = to_string(player1.left - 1) +
			to_string(player1.right - 1);

		player0.sendtoClient(DATA);
		player1.sendtoClient(DATA);
		usleep(500);

		player0.sendtoClient(s0 + s1);
		player1.sendtoClient(s1 + s0);
		usleep(500);
	}

	void start(void)
	{
		player0.sendtoClient(START);
		player1.sendtoClient(START);
		usleep(500);
	}

	void loop(void)
	{
		string data;

		while (playerHasMove()) {
			player0.sendtoClient(MOVE);
			player1.sendtoClient(WAIT);
			usleep(500);

			do {
				player0.recvfromClient(data);
				usleep(500);
				if (data == QUIT)
					return;
			} while (!isValid(data));

			playerMakesMove(data);

			status();
			swap(player0, player1);
		}
	}

	void end(void)
	{
		player0.sendtoClient(LOST);
		player1.sendtoClient(WON);
		usleep(500);
	}

	bool isValid(const string& data)
	{
		if (data == LEFT2LEFT &&
				player0.left + player1.left <= 10)
			return true;
		if (data == LEFT2RIGHT &&
				player0.left + player1.right <= 10)
			return true;
		if (data == RIGHT2LEFT &&
				player0.right + player1.left <= 10)
			return true;
		if (data == RIGHT2RIGHT &&
				player0.right + player1.right <= 10)
			return true;
		player0.sendtoClient(ERROR);
		usleep(500);
		return false;
	}

	bool playerHasMove()
	{
		if (player0.left + player1.left <= 10)
			return true;
		if (player0.left + player1.right <= 10)
			return true;
		if (player0.right + player1.left <= 10)
			return true;
		if (player0.right + player1.right <= 10)
			return true;
		return false;
	}

	void playerMakesMove(const string& data)
	{
		if (data == LEFT2LEFT)
			player0.left += player1.left;
		else if (data == LEFT2RIGHT)
			player0.left += player1.right;
		else if (data == RIGHT2LEFT)
			player0.right += player1.left;
		else if (data == RIGHT2RIGHT)
			player0.right += player1.right;
	}

private:
	Player player0;
	Player player1;
};

}

#endif
