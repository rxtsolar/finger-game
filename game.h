#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <vector>

using namespace std;

namespace gs {

class Player {
public:
	int left;
	int right;
	Player(void) : left(1), right(1) { }
};

class Game {
public:
	Game(int n) : turn(0), players(n) { }
	void start(void)
	{
		while (playerHasMove()) {
			playerMakesMove();
			turn = (turn + 1) % 2;
		}
	}

private:
	int turn;
	vector<Player> players;

	bool playerHasMove()
	{
		if (players[turn].left + players[1 - turn].left <= 10)
			return true;
		if (players[turn].left + players[1 - turn].right <= 10)
			return true;
		if (players[turn].right + players[1 - turn].left <= 10)
			return true;
		if (players[turn].right + players[1 - turn].right <= 10)
			return true;

		cout << "No valid moves left!" << endl;
		cout << "Player " << 1 - turn << " has won the game!" << endl;
		return false;
	}

	void playerMakesMove()
	{
		while (true) {
			string selfHand;
			string playerHand;
			int player;
			int selfNumber;
			int playerNumber;

			cout << "*********************************" << endl;
			for (int i = 0; i < 2; i++)
				cout << "* Player " << i << ": left " << players[i].left << ", right " << players[i].right << endl;
			cout << "*********************************" << endl;
			cout << "Player " << turn << "'s move: " << endl;

			cin >> selfHand >> playerHand;

			if (selfHand != "left" && selfHand != "right")
				continue;
			if (playerHand != "left" && playerHand != "right")
				continue;

			if (selfHand == "left")
				selfNumber = players[turn].left;
			else if (selfHand == "right")
				selfNumber = players[turn].right;
			if (playerHand == "left")
				playerNumber = players[1 - turn].left;
			else if (playerHand == "right")
				playerNumber = players[1 - turn].right;

			if (selfNumber + playerNumber <= 10) {
				if (selfHand == "left")
					players[turn].left += playerNumber;
				else if (selfHand == "right")
					players[turn].right += playerNumber;
				break;
			}
		}
	}
};

}

#endif
