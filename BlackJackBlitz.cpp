#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum Rank {
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE
};

// Associate the enums with their string names
const map<Suit, string> suitNames = {
    {Suit::HEARTS, "Hearts"},
    {Suit::DIAMONDS, "Diamonds"},
    {Suit::CLUBS, "Clubs"},
    {Suit::SPADES, "Spades"}
};

const map<Rank, string> rankNames = {
    {Rank::TWO, "2"},
    {Rank::THREE, "3"},
    {Rank::FOUR, "4"},
    {Rank::FIVE, "5"},
    {Rank::SIX, "6"},
    {Rank::SEVEN, "7"},
    {Rank::EIGHT, "8"},
    {Rank::NINE, "9"},
    {Rank::TEN, "10"},
    {Rank::JACK, "Jack"},
    {Rank::QUEEN, "Queen"},
    {Rank::KING, "King"},
    {Rank::ACE, "Ace"}
};

class Card {
private:
    Suit suit;
    Rank rank;

public:
    Card(Suit suit, Rank rank) {
        this->suit = suit;
        this->rank = rank;
    }

    Suit getSuit() {
        return suit;
    }

    Rank getRank() {
        return rank;
    }

    string toString() {
        return rankNames.at(rank) + " of " + suitNames.at(suit);
    }
};

class Deck {
private:
    vector<Card> cards;

public:
    Deck() {
        reset();
    }

    void reset() {
        cards.clear();
        for (Suit suit : {HEARTS, DIAMONDS, CLUBS, SPADES}) {
            for (Rank rank : {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE}) {
                cards.push_back(Card(suit, rank));
            }
        }
    }

    void shuffle() {
        random_shuffle(cards.begin(), cards.end());
    }

    Card dealCard() {
        if (!cards.empty()) {
            Card card = cards.back();
            cards.pop_back();
            return card;
        }
        return Card(HEARTS, TWO); 
    }

    bool isEmpty() {
        return cards.empty();
    }
};

class Hand {
private:
    vector<Card> cards;

public: //clearing
    void clear() {
        cards.clear();
    }
    Hand() {}

    void addCard(Card card) {
        cards.push_back(card);
    }

    int getValue() {
        int value = 0;
        int numAces = 0;
        for (Card card : cards) {
            Rank rank = card.getRank();
            if (rank == Rank::ACE) {
                numAces++;
            } else if (rank >= Rank::TEN) {
                value += 10;
            } else {
                value += rank + 2; //add ng dalawa para iconvert ang rank enum sa kanyang value
            }
        }

        for (int i = 0; i < numAces; i++) {
            if (value + 11 <= 21) {
                value += 11;
            } else {
                value += 1;
            }
        }
        return value;
    }

    string toString() {
        string result;
        for (Card card : cards) {
            result += card.toString() + ", ";
        }
        return result;
    }

    Card getCard(int index) {
        if (index >= 0 && index < cards.size()) {
            return cards[index];
        } else {
            return cards[0];
        }
    }
};

class Player {
protected:
    Hand hand;

public:
    void addCardToHand(Card card) {
        hand.addCard(card);
    }

    Hand getHand() {
        return hand;
    }

    virtual void play(Deck& deck) = 0;
};

class Dealer : public Player {
public:
    void clearHand() {
        hand.clear();
    }
    
    void play(Deck& deck) {
        while (hand.getValue() < 17) {
            addCardToHand(deck.dealCard());
        }
    }

    void displayFirstCard() {
        cout << "\n ";
        cout << "\n ";
        cout << "\n";
        cout << "   Dealer's first card: " + hand.getCard(0).toString() << endl;
    }
};

class User : public Player {
public:
    void clearHand() {
        hand.clear();
    }

    void play(Deck& deck) {
        while (true) {
            cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "   Do you want to (H)it or (S)tand? ";
            string choice;
            cin >> choice;
            if (choice == "H") {
                addCardToHand(deck.dealCard());
                cout << "   Your hand: " + hand.toString() << endl;
                if (hand.getValue() > 21) {
                    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
                    cout << "   You busted!" << endl;
                    break;
                }
            } else if (choice == "S") {
                break;
            } else {
                cout << "   Invalid choice. Please choose again." << endl;
            }
        }
    }
};

class BlackjackGame {
private:
    int playerBalance;
    int currentBet;
    Deck deck;
    Dealer dealer;
    User user;

public:
    BlackjackGame(int initialBalance) : playerBalance(initialBalance) {}

    void play() {
        deck.shuffle();

        // para maclear
        user.clearHand();
        dealer.clearHand();

        dealer.addCardToHand(deck.dealCard());
        dealer.addCardToHand(deck.dealCard());
        user.addCardToHand(deck.dealCard());
        user.addCardToHand(deck.dealCard());

        dealer.displayFirstCard();
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "   Your hand: " + user.getHand().toString() << endl;

        user.play(deck);

        if (user.getHand().getValue() <= 21) {
            dealer.play(deck);
            cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "   Dealer's hand: " + dealer.getHand().toString() << endl;
            int userValue = user.getHand().getValue();
            int dealerValue = dealer.getHand().getValue();
            
            if (dealerValue > 21 || userValue > dealerValue) {
                cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
                cout << "   ░█▀▀░█▀█░█▀█░█▀▀░█▀▄░█▀█░▀█▀░█░█░█░░░█▀█░▀█▀░▀█▀░█▀█░█▀█░█▀▀░█░░░█░█░█▀█░█░█░░░█░█░▀█▀░█▀█░█" << endl;
                cout << "   ░█░░░█░█░█░█░█░█░█▀▄░█▀█░░█░░█░█░█░░░█▀█░░█░░░█░░█░█░█░█░▀▀█░▀░░░░█░░█░█░█░█░░░█▄█░░█░░█░█░▀" << endl;
                cout << "   ░▀▀▀░▀▀▀░▀░▀░▀▀▀░▀░▀░▀░▀░░▀░░▀▀▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀▀▀░▀░▀░▀▀▀░▀░░░░▀░░▀▀▀░▀▀▀░░░▀░▀░▀▀▀░▀░▀░▀" << endl;
                playerBalance += 2 * currentBet;
                
            } else if (userValue < dealerValue) {
                cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
                cout << "   ░█▀▄░█▀▀░█▀█░█░░░█▀▀░█▀▄░░░█░█░▀█▀░█▀█░█▀▀░█" << endl;
                cout << "   ░█░█░█▀▀░█▀█░█░░░█▀▀░█▀▄░░░█▄█░░█░░█░█░▀▀█░▀" << endl;
                cout << "   ░▀▀░░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀░▀░░░▀░▀░▀▀▀░▀░▀░▀▀▀░▀" << endl;
                
            } else {
                cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
                cout << "   ░▀█▀░▀█▀░▀░█▀▀░░░█▀█░░░▀█▀░▀█▀░█▀▀░█" << endl;
                cout << "   ░░█░░░█░░░░▀▀█░░░█▀█░░░░█░░░█░░█▀▀░▀" << endl;
                cout << "   ░▀▀▀░░▀░░░░▀▀▀░░░▀░▀░░░░▀░░▀▀▀░▀▀▀░▀" << endl;
                playerBalance += currentBet;
            }
            
        } else {
            cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
            cout << "   Dealer wins!" << endl;
        }
        
        cout << "   Remaining balance: " << playerBalance << endl;
    }
    
    void placeBet() {
        cout << " " << endl;
        cout << " " << endl;
        cout << " " << endl;
        
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "   Place your bet (current balance: " << playerBalance << "): ";
        
        int bet;
        do {
            while (!(cin >> bet)) {
                cout << "   Invalid input. Please enter a valid integer." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (bet > playerBalance) {
                cout << "   You cannot bet more than your current balance." << endl;
                cout << "   Place your bet (current balance: " << playerBalance << "): ";
            }
        } while (bet > playerBalance);
        
        currentBet = bet;
        playerBalance -= bet;
        
        char choice;
        cout << "   Do you want to change your bet? (Y/N): ";
        cin >> choice;
        cout << "\n--------------------------------------------------------------------------------------------------------------------" << endl;
        
        if (choice == 'Y' || choice == 'y') {
            cout << "   Enter your new bet (current bet: " << currentBet << ", current balance: " << playerBalance << "): ";
            int newBet;
            do {
                while (!(cin >> newBet)) {
                    cout << "   Invalid input. Please enter a valid integer." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (newBet > playerBalance) {
                    cout << "   You cannot bet more than your current balance." << endl;
                    cout << "   Enter your new bet (current bet: " << currentBet << ", current balance: " << playerBalance << "): ";
                }
            } while (newBet > playerBalance);
            
            playerBalance += currentBet;
            currentBet = newBet;
            playerBalance -= newBet;
        }
    }

    void displayRemainingBalance() {
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "   Remaining balance: " << playerBalance << endl;
    }
};


class Menu {
public:
    void displayMenu() {
        cout << " " << endl;
        cout << " " << endl;
        cout << " " << endl;

        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "|                                                     ░█▄█░█▀▀░█▀█░█░█                                             |" << endl;
        cout << "|                                                     ░█░█░█▀▀░█░█░█░█                                             |" << endl;
        cout << "|                                                     ░▀░▀░▀▀▀░▀░▀░▀▀▀                                             |" << endl;
        cout << "|                                                         1. Play                                                  |" << endl;
        cout << "|                                                         2. How?                                                  |" << endl;
        cout << "|                                                         3. Replay Game                                           |" << endl;
        cout << "|                                                         4. Quit                                                  |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    }

    int getChoice() {
        int choice;
        cout << " " << endl;
        cout << " " << endl;
        cout << " " << endl;

        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "   Please enter your choice: ";
        cin >> choice;
        return choice;
    }
};

class GameInstructions {
public:
    void displayInstructions() {
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "|                                                        ░█░█░█▀█░█░█                                               |" << endl;
        cout << "|                                                        ░█▀█░█░█░█▄█                                               |" << endl;
        cout << "|                                                        ░▀░▀░▀▀▀░▀░▀                                               |" << endl;
        cout << "|   1. This is a standard game of Blackjack played with a standard deck of 52 cards.                                |" << endl;
        cout << "|   2. Each player starts with 2 cards, and the dealer reveals one of their cards.                                  |" << endl;
        cout << "|   3. Players decide whether to HIT (take another card) or STAND (end their turn).                                 |" << endl;
        cout << "|   4. Numbered cards are worth their face value, and face cards (KING, QUEEN, JACK) are worth 10 points.           |" << endl;
        cout << "|   5. Aces can be worth either 1 or 11 points, depending on the player's choice.                                   |" << endl;
        cout << "|   6. The player wins if their hand's total is closer to 21 than the dealer's without exceeding 21.                |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    }
};

class ReplayGame {
public:
    void replay(int& playerBalance) {
        cout << "░█▀▀░▀█▀░█▀█░█▀▄░▀█▀░▀█▀░█▀█░█▀▀░░░█▀█░█▀▀░█░█░░░█▀▄░█▀█░█░█░█▀█░█▀▄░░░░░░░░░" << endl;
        cout << "░▀▀█░░█░░█▀█░█▀▄░░█░░░█░░█░█░█░█░░░█░█░█▀▀░█▄█░░░█▀▄░█░█░█░█░█░█░█░█░░░░░░░░░" << endl;
        cout << "░▀▀▀░░▀░░▀░▀░▀░▀░░▀░░▀▀▀░▀░▀░▀▀▀░░░▀░▀░▀▀▀░▀░▀░░░▀░▀░▀▀▀░▀▀▀░▀░▀░▀▀░░▀░░▀░░▀░" << endl;
        BlackjackGame game(playerBalance);
        game.placeBet();
        game.play();
        game.displayRemainingBalance();
    }
};

int main() {
    cout << "                                                ░█░█░█▀▀░█░░░█▀▀░█▀█░█▄█░█▀▀░░░▀█▀░█▀█░░░░                                " << endl;
    cout << "                                                ░█▄█░█▀▀░█░░░█░░░█░█░█░█░█▀▀░░░░█░░█░█░░░                                 " << endl;
    cout << "                                                ░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀░░░░▀░░▀▀▀░░░░                                " << endl;
    
    cout << "           ██████╗ ██╗      █████╗  ██████╗██╗  ██╗     ██╗ █████╗  ██████╗██╗  ██╗   ██████╗ ██╗     ██╗████████╗███████╗" << endl;
    cout << "           ██╔══██╗██║     ██╔══██╗██╔════╝██║ ██╔╝     ██║██╔══██╗██╔════╝██║ ██╔╝   ██╔══██╗██║     ██║╚══██╔══╝╚══███╔╝" << endl;
    cout << "           ██████╔╝██║     ███████║██║     █████╔╝      ██║███████║██║     █████╔╝    ██████╔╝██║     ██║   ██║     ███╔╝ " << endl;
    cout << "           ██╔══██╗██║     ██╔══██║██║     ██╔═██╗ ██   ██║██╔══██║██║     ██╔═██╗    ██╔══██╗██║     ██║   ██║    ███╔╝  " << endl;
    cout << "           ██████╔╝███████╗██║  ██║╚██████╗██║  ██╗╚█████╔╝██║  ██║╚██████╗██║  ██╗   ██████╔╝███████╗██║   ██║   ███████╗" << endl;
    cout << "           ╚═════╝ ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚════╝ ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝   ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚══════╝" << endl;

    int playerBalance = 1000; 

    Menu menu;
    BlackjackGame game(playerBalance);
    GameInstructions how;
    ReplayGame restart;

    while (true) {
        menu.displayMenu();
        int choice = menu.getChoice();
        switch (choice) {
            case 1:
                game.placeBet();
                game.play();
                game.displayRemainingBalance();
                break;
            case 2:
                how.displayInstructions();
                break;
            case 3:
                restart.replay(playerBalance);
                break;
            case 4:
                cout << "░█▀▀░█░█░▀█▀░▀█▀░▀█▀░█▀█░█▀▀░░░▀█▀░█░█░█▀▀░░░█▀▀░█▀█░█▄█░█▀▀░░░░░░█▀▀░█▀█░█▀█░█▀▄░█▀▄░█░█░█▀▀░█"<<endl;
                cout << "░█▀▀░▄▀▄░░█░░░█░░░█░░█░█░█░█░░░░█░░█▀█░█▀▀░░░█░█░█▀█░█░█░█▀▀░░░░░░█░█░█░█░█░█░█░█░█▀▄░░█░░█▀▀░▀"<<endl;
                cout << "░▀▀▀░▀░▀░▀▀▀░░▀░░▀▀▀░▀░▀░▀▀▀░░░░▀░░▀░▀░▀▀▀░░░▀▀▀░▀░▀░▀░▀░▀▀▀░▀░░░░▀▀▀░▀▀▀░▀▀▀░▀▀░░▀▀░░░▀░░▀▀▀░▀" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please choose again." << endl;
                break;
        }
    }
    return 0;
}
