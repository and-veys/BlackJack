#include <iostream>
#include <string.h>
#include <vector>
#include <ctime>
#include <cassert>
#define MAX_NAME 15
enum SUIT{hearts = 3, diamonds, clubs, spades};
enum RANK{rank_A = 1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_J, rank_Q, rank_K};
enum GAME{hide, start, end_win, end_lose, end_push};
//------------------------------------------------------------------------
int getPlayersCount(std::string question, int nMax)
{	
	while(true)
	{
		std::string buffer;
		std::cout << question << " ([1] - " << nMax << "): ";		
		std::getline(std::cin, buffer);		
		if(buffer.size() == 0) 
			return 1;
		int res = 0;
		int i;
		for(i=0; i<buffer.size(); ++i)
		{
			if(buffer[i] >= '0' && buffer[i] <= '9')
				res = res*10 + buffer[i] - '0';
			else break;		
		}
		if(i == buffer.size() && (res >= 1 && res <= nMax)) 
			return res;
		std::cout << "Error!! ";
	}
	assert(false);
	return -1;	
}

bool isYes(std::string question)
{
	std::string buffer;	
	std::cout << std::endl;	
	while(true)
	{
		std::cout << question << " ([Y]/N): ";
		std::getline(std::cin, buffer);
		if(buffer.size() == 0 || buffer == "y" || buffer == "Y") return true;
		if(buffer == "n" || buffer == "N") return false;
	}
	assert(false);
	return false;
}
template <class T>
void vector_mix(std::vector<T> & vec)
{
	T temp;
	int n;
	for(int i=0; i<vec.size(); ++i)
	{
		n = rand()%vec.size();
		temp = vec[n];
		vec[n] = vec[i];
		vec[i] = temp;
	}
}
//------------------------------------------------------------------------
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
	friend std::ostream& operator << (std::ostream& out, Card & aC);
};
std::ostream& operator << (std::ostream& out, Card & aC)
{
	if(aC.m_isFaceUp)
	{
		const std::string temp[] = { " 0", " A", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9","10", " J", " Q", " K" };
		out << temp[aC.m_rank] << (unsigned char)aC.m_suit;
	}
	else
		out << "???";
	return out;
}

/*
std::string Card::getCardString()
{
	std::string res;
	if() return "   ";
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
		default: assert(false);
	}
	return (res + (char)m_suit);	
};
*/
//------------------------------------------------------------------------
class Hand
{
protected:
	std::vector<Card*> m_cards;
public:
	Hand(){};
	void add(Card* c){m_cards.push_back(c);}
	void clear(){m_cards.clear();}
	int getTotal();
	void flip(){for(int i=0; i<m_cards.size(); ++i) m_cards[i]->flip();}
	std::vector<Card*> & getCards(){return m_cards;}		//xxx
};
int Hand::getTotal()
{
	int res = 0;
	bool A = false;
	for(int i=0; i<m_cards.size(); ++i)
	{
		A = A || (m_cards[i]->getRank() == RANK::rank_A);
		res+=m_cards[i]->getValue();
	}
	if(A && res <= 11) res+=10;
	return res;	
}
/*
void Hand::print(std::string s = "", std::string e = "")
{	
	unsigned char a[] = {201, 205, 205, 205, 187, 0};
	unsigned char b[] = {200, 205, 205, 205, 188, 0};
	unsigned char c = 186;
	std::cout << std::string(s.size()+1, 32);
	for(int i=0; i<m_cards.size(); ++i) 
		std::cout << a;	
	std::cout << std::endl << s << " ";	
	for(int i=0; i<m_cards.size(); ++i) 
		std::cout << c << m_cards[i]->getCardString() << c;
	std::cout << " " << e << std::endl << std::string(s.size()+1, 32);
	for(int i=0; i<m_cards.size(); ++i) 
		std::cout << b;
	std::cout << std::endl;
}*/
//------------------------------------------------------------------------
class Deck : public Hand
{
public:
	Deck():m_currentCard(0) {populate();}
	~Deck(){for(int i=0; i<m_cards.size(); ++i) delete m_cards[i];}
	void shuffle(){m_currentCard = 0; vector_mix<Card*>(m_cards);}
	void deal(Hand * aH){aH->getCards().push_back(m_cards[m_currentCard]); m_cards[m_currentCard++]->flip();}
private:
	void populate();
	int m_currentCard;
};
void Deck::populate()
{
	m_cards.resize(52);
	int k = 0;
	for(int i=hearts; i<=spades; ++i)
	{
		for(int j=rank_A; j<=rank_K; ++j)
			m_cards[k++] = new Card((RANK)j, (SUIT)i);
	}
}
//--------------------------------
class GenericPlayer : public Hand
{
protected:
	std::string m_name;	
	int m_score;
	GAME m_resultOfTurn;
public:	
	GenericPlayer(std::string n):m_name(n), m_score(100), m_resultOfTurn(hide){};	
	virtual bool isHitting() = 0;
	friend std::ostream & operator << (std::ostream & out, GenericPlayer & aGP);
	GenericPlayer & operator ++ (){++m_score; return *this;};
	GenericPlayer & operator -- (){--m_score; return *this;};
	virtual void startOfTurn(){flip(); clear();}
	virtual void endOfTurn(GenericPlayer * aGP);
	bool isBusted(){return getTotal() > 21;}
private:
	std::string bust(){return "BUST !!";}
	std::string win(){return "WIN !!";}
	std::string lose(){return "LOSE !!";}
	std::string push(){return "PUSH !!";}
	std::string points(){return std::to_string(getTotal()) + " points";}
	std::string playAway(){return "PLAY AWAY !!";};
};
std::ostream & operator <<(std::ostream & out, GenericPlayer & aGP)
{	
	unsigned char a[] = {201, 205, 205, 205, 187, 0};
	unsigned char b[] = {200, 205, 205, 205, 188, 0};
	unsigned char c = 186;
	std::string temp;
	if(aGP.m_name.size() < MAX_NAME)
		temp = aGP.m_name + std::string(MAX_NAME - aGP.m_name.size(), '.') + " ";
	else
		temp = aGP.m_name.substr(0, MAX_NAME);	
	out << std::string(temp.size(), ' ');
		for(int i=0; i<aGP.m_cards.size(); ++i) 
			out << a;	
	out << std::endl << temp;	
	for(int i=0; i<aGP.m_cards.size(); ++i) 
		out << c << *(aGP.m_cards[i]) << c;
	out << " ";
	switch(aGP.m_resultOfTurn)
	{
		case end_win:
			out << aGP.win();
			break;
		case end_lose:
			out << aGP.lose();
			break;
		case end_push:
			out << aGP.push();
			break;
		case start:
			if(aGP.isBusted()) out << aGP.bust();
			else out << aGP.points();			
	}		
	out << std::endl << std::string(temp.size(), ' ');
	for(int i=0; i<aGP.m_cards.size(); ++i) 
		out << b;
	out << std::endl;
	return out;
}
void GenericPlayer::endOfTurn(GenericPlayer * house)
{
	if((isBusted() && house->isBusted()) || (getTotal() == house->getTotal()))
		m_resultOfTurn = end_push;
	else
	{
		if(isBusted()) 
			m_resultOfTurn = end_lose;
		else if(house->isBusted())
			m_resultOfTurn = end_win;
		else
			m_resultOfTurn = (getTotal() > house->getTotal())?end_win:end_lose;	
		if(m_resultOfTurn == end_win)
		{
			++(*this);
			--(*house);			
		}
		else
		{
			--(*this);
			++(*house);	
		}
	}
}
//------------------------------------------------------------------------
class Player_Human : public GenericPlayer
{
public:
	Player_Human(std::string n):GenericPlayer(n){}
	void startOfTurn(){GenericPlayer::startOfTurn(); m_resultOfTurn = start;}
private:
	bool isHitting(){return false;}
	
};

//------------------------------------------------------------------------
class Player_AI : public GenericPlayer
{
public:
	Player_AI(std::string n):GenericPlayer(n){}
private:
	bool isHitting(){return false;}
};
//------------------------------------------------------------------------
class House : public GenericPlayer
{
public:
	House():GenericPlayer("[HOUSE]"){}
	void flipFirstCard(){m_cards[0]->flip();}
	void endOfTurn(GenericPlayer * pl){flipFirstCard();}
private:
	bool isHitting(){return false;}
};
//------------------------------------------------------------------------
class Game
{
private:
	Deck m_deck;
	std::vector<GenericPlayer*> m_players; //Player_Human + Player_AI + House
public:
	Game(std::vector <std::string> & pl_h, std::vector <std::string> & pl_ai);
	~Game(){for(int i=0; i<m_players.size(); ++i) delete m_players[i];}
	friend std::ostream & operator << (std::ostream & out, Game & aG);
	void play();	
};
Game::Game(std::vector <std::string> & pl_h, std::vector <std::string> & pl_ai)
{
	m_players.resize(pl_h.size()+pl_ai.size());
	for(int i=0; i<pl_h.size(); ++i) m_players[i] = new Player_Human(pl_h[i]);
	for(int i=0; i<pl_ai.size(); ++i) m_players[i+pl_h.size()] = new Player_AI(pl_ai[i]);
	vector_mix<GenericPlayer*>(m_players);
	m_players.push_back(new House());
}
std::ostream & operator << (std::ostream & out, Game & aG)
{
	system("cls");
	for(int i=0; i<aG.m_players.size(); ++i) 
		out << *(aG.m_players[i]);	
	return out;
}
void Game::play()		
{	
	House * house = ((House *)m_players.back());
	m_deck.shuffle();
	for(int i=0; i<m_players.size(); ++i)
		m_players[i]->startOfTurn();
	for(int j=0; j<2; ++j)
	{
		for(int i=0; i<m_players.size(); ++i) 
			m_deck.deal(m_players[i]);
	}	
	house->flipFirstCard();
	std::cout << *this;	
	for(int i=0; i<m_players.size(); ++i)
	{
		while(m_players[i]->isHitting())
		{
			m_deck.deal(m_players[i]);			
			std::cout << *this;
		}
	}
	for(int i=0; i<m_players.size(); ++i)
		m_players[i]->endOfTurn(house);
	std::cout << *this;
}
//------------------------------------------------------------------------
int main()
{
	srand(time(NULL));
	system("cls");
	std::cout << "\t\tWelcome to Blackjack!\n\n";  
    int numPlayers = getPlayersCount("How many players?", 7);	
	int numHumans = (numPlayers == 1)?1:getPlayersCount("How many humans of them?", numPlayers);	
	std::string buffer;
    std::vector <std::string> names_H = {"Ivan", "Andrey", "Sergey", "Alexey", "Denis", "Roman", "Vladimir"};
	std::vector <std::string> names_AI(numPlayers-numHumans);
	vector_mix(names_H);
    for(int i = 0; i < numHumans; ++i)
    {		
		std::cout << "Enter name of player #" << i+1 << " [" << names_H[i] << "] (max: " << MAX_NAME << " characters): ";
		std::getline(std::cin, buffer);
		if(buffer.size() != 0)
			names_H[i] = buffer;
	}
	names_H.resize(numHumans);
	for(int i=0; i<names_AI.size(); ++i)
		names_AI[i] = "<COMPUTER " + std::to_string(i+1) + ">";
	Game aGame(names_H, names_AI);
	do
	{
		aGame.play();		
	} 
	while(isYes("Do you want to play again?"));
	system("cls");
	return 0;
} 