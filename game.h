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
	Game(int n) : total(n), turn(0), players(n) { }
	void start(void)
	{
		while (playerHasMove(turn)) {
			playerMakesMove(turn);
			turn = (turn + 1) % total;
		}
	}

private:
	int total;
	int turn;
	vector<Player> players;

	bool playerHasMove(int t)
	{
		for (int i = 0; i < total; i++) {
			if (i == t)
				continue;
			if (players[t].left + players[i].left <= 10)
				return true;
			if (players[t].left + players[i].right <= 10)
				return true;
			if (players[t].right + players[i].left <= 10)
				return true;
			if (players[t].right + players[i].right <= 10)
				return true;
		}
		cout << "No valid moves left!" << endl;
		cout << "Player " << t << " has lost the game!" << endl;
		return false;
	}

	void playerMakesMove(int t)
	{
		while (true) {
			string selfHand;
			string playerHand;
			int player;
			int selfNumber;
			int playerNumber;

			cout << "*********************************" << endl;
			for (int i = 0; i < total; i++)
				cout << "* Player " << i << ": left " << players[i].left << ", right " << players[i].right << endl;
			cout << "*********************************" << endl;
			cout << "Player " << t << "'s move: " << endl;

			cin >> selfHand >> player >> playerHand;
			if (player < 0 || player >= total)
				continue;
			if (selfHand != "left" && selfHand != "right")
				continue;
			if (playerHand != "left" && playerHand != "right")
				continue;

			if (selfHand == "left")
				selfNumber = players[t].left;
			else if (selfHand == "right")
				selfNumber = players[t].right;
			if (playerHand == "left")
				playerNumber = players[player].left;
			else if (playerHand == "right")
				playerNumber = players[player].right;

			if (selfNumber + playerNumber <= 10) {
				if (selfHand == "left")
					players[t].left += playerNumber;
				else if (selfHand == "right")
					players[t].right += playerNumber;
				break;
			}
		}
	}
};

}

#endif
