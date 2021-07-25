#include <iostream>
#include <string.h>
#include <vector>
#define MAX_NAME 10
enum SUIT{hearts = 3, diamonds, clubs, spades};
enum RANK{rank_A = 1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_J, rank_Q, rank_K};
class Card
{
private:
	SUIT m_suit;
	RANK m_rank;
	bool m_isFaceUp;
public:
	Card(RANK r, SUIT s, bool u = false): m_rank(r), m_suit(s), m_isFaceUp(u){}
	void flip() {m_isFaceUp = !m_isFaceUp;}
	RANK getRank() {return m_rank;}
	int getValue() {if(m_rank > 10) return 10; return m_rank;}
	std::string getCardString();
};
class Hand
{
private:
	std::vector<Card*> m_cards;
public:
	void add(Card* c){m_cards.push_back(c);}
	void clear(){m_cards.clear();};
	int getTotal();
	void print(std::string s1);
};
std::string Card::getCardString()
{
	std::string res;
	switch(m_rank)
	{
		case rank_2: res = " 2"; break;
		case rank_3: res = " 3"; break;
		case rank_4: res = " 4"; break;
		case rank_5: res = " 5"; break;
		case rank_6: res = " 6"; break;
		case rank_7: res = " 7"; break;
		case rank_8: res = " 8"; break;
		case rank_9: res = " 9"; break;
		case rank_10: res = "10"; break;
		case rank_J: res = " J"; break;
		case rank_Q: res = " Q"; break;
		case rank_K: res = " K"; break;
		case rank_A: res = " A"; break;
		default: res = "ERROR";
	}
	return (res + (char)m_suit);	
};
int Hand::getTotal()
{
	int res = 0;
	bool A = false;
	for(int i=0; i<m_cards.size(); i++)
	{
		A = A || (m_cards[i]->getRank() == RANK::rank_A);
		res+=m_cards[i]->getValue();
	}
	if(A && res <= 11) res+=10;
	return res;	
}
void Hand::print(std::string s = "")
{	
	unsigned char a[] = {201, 205, 205, 205, 187, 0};
	unsigned char b[] = {200, 205, 205, 205, 188, 0};
	unsigned char c = 186;
	std::cout << std::string(MAX_NAME, 32);
	for(int i=0; i<m_cards.size(); i++) 
		std::cout << a;
	std::cout << std::endl << s;
	for(int i=0; i<m_cards.size(); i++) 
		std::cout << c << m_cards[i]->getCardString() << c;
	std::cout << std::endl << std::string(MAX_NAME, 32);
	for(int i=0; i<m_cards.size(); i++) 
		std::cout << b;
	std::cout << std::endl;
}
int main()
{
	system("cls");
	std::cout << "\t\tWelcome to Blackjack!\n\n";    
	std::string buffer;
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        std::cout << "How many players? (1 - 7): ";
        std::cin >> buffer;
		try {numPlayers = std::stoi(buffer);}
		catch(std::invalid_argument e) {numPlayers = 0;}
    }    
    std::vector <std::string> names;
    for (int i = 0; i < numPlayers; ++i)
    {
        std::cout << "Enter name of player #" << i+1 << " (max: " << MAX_NAME << " characters): ";
        std::cin >> buffer;		
		if(buffer.size() < MAX_NAME)
			names.push_back(std::string(MAX_NAME, '.').replace(0, buffer.size(), buffer));
		else
			names.push_back (std::string(MAX_NAME, '.').replace(0, MAX_NAME, buffer, 0, MAX_NAME));
    }
    system("cls");
//	Game aGame(names);
	buffer = "";
    while (buffer != "n" && buffer != "N")
    {
//		aGame.Play();
        std::cout << "\nDo you want to play again? (Y/N): ";
		std::cin >> buffer;
    }
/*
	Card c1(rank_3, hearts);
	Card c2(rank_10, hearts);
	Card c3(rank_A, hearts);
	Hand h, h1;
	h.add(&c1); h.add(&c2); h.add(&c3);
	h1.add(&c1); h1.add(&c2); h1.add(&c3);
	h.print(names[0]);	
	h1.print(names[1]);
*/		
	return 0;
} 