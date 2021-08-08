#include <iostream>
#include <string.h>
#include <vector>
#include <ctime>
#include <cassert>
#define MAX_NAME 15
#define MAX_SCORE 21
enum SUIT{hearts = 3, diamonds, clubs, spades};
enum RANK{rank_A = 1, rank_2, rank_3, rank_4, rank_5, rank_6, rank_7, rank_8, rank_9, rank_10, rank_J, rank_Q, rank_K};
enum STATUS{turn, end_win, end_lose, end_push, play_away};
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
template <class T> void vector_mix(std::vector<T> & vec)
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
	bool isFaceUp(){return m_isFaceUp;}
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
//------------------------------------------------------------------------
class Deck : public Hand
{
public:
	Deck():m_currentCard(0) {populate();}
	~Deck(){for(int i=0; i<m_cards.size(); ++i) delete m_cards[i];}
	void shuffle(){m_currentCard = 0; vector_mix<Card*>(m_cards);}
	void deal(Hand * aH){aH->add(m_cards[m_currentCard]); m_cards[m_currentCard++]->flip();}
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
//------------------------------------------------------------------------
class GenericPlayer : public Hand
{
protected:
	std::string m_name;	
	int m_score;
	STATUS m_status;
public:	
	GenericPlayer(std::string n):m_name(n), m_score(MAX_SCORE){};	
	virtual bool isHitting(Card *) = 0;
	friend std::ostream & operator << (std::ostream & out, GenericPlayer & aGP);
	GenericPlayer & operator ++ (){++m_score; return *this;};
	GenericPlayer & operator -- (){if(m_score != 0) --m_score; return *this;};
	void startOfTurn(){flip(); clear(); m_status = (m_score <= 0?play_away:turn);}
	void endOfTurn(GenericPlayer * pl);
	bool isBusted(){return getTotal() > 21;}
	bool isPlayAway(){return m_status == play_away;}	
	bool isCardsUp();
private:
	std::string bust(){return "BUST !!";}
	std::string win(){return "WIN !!";}
	std::string lose(){return "LOSE !!";}
	std::string push(){return "PUSH !!";}
	std::string blackjack(){return "BLACKJACK !!";}
	std::string points(){return (getTotal()==21?blackjack():std::to_string(getTotal()) + " points");}
	std::string score(){return ((m_score < 10?" ":"") + std::to_string(m_score) + "$");};
	std::string playAway(){return "PLAY AWAY !!";}	
};
bool GenericPlayer::isCardsUp()
{
	for(int i=0; i<m_cards.size(); ++i)
	{
		if(!m_cards[i]->isFaceUp()) return true;
	}
	return false;
	
}
std::ostream & operator <<(std::ostream & out, GenericPlayer & aGP)
{	
	std::string a(aGP.isPlayAway()?aGP.playAway().size()+2:5, 205); a[0] = 201; a[a.size()-1]=187;
	std::string b(1, 186);
		std::string temp;
	if(aGP.m_name.size() < MAX_NAME)
		temp = aGP.m_name + std::string(MAX_NAME - aGP.m_name.size(), '.');
	else
		temp = aGP.m_name.substr(0, MAX_NAME);
	temp+=" : " + aGP.score() + " ";		
	if(aGP.isPlayAway())
	{
		out << std::string(temp.size(), ' ') << a << std::endl;
		out << temp << b << aGP.playAway() << b << std::endl;
		a[0] = 200; a[a.size()-1]=188;
		out << std::string(temp.size(), ' ') << a << std::endl;
	}		
	else
	{
		out << std::string(temp.size(), ' ');
		for(int i=0; i<aGP.m_cards.size(); ++i) 
			out << a;
		out << std::endl << temp;	
		for(int i=0; i<aGP.m_cards.size(); ++i) 
			out << b << *(aGP.m_cards[i]) << b;		
		out << " ";
		switch(aGP.m_status)
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
			case turn:
				if(!aGP.isCardsUp())
				{
					if(aGP.isBusted()) out << aGP.bust();
					else out << aGP.points();
				}			
		}	
		out << std::endl << std::string(temp.size(), ' ');
		a[0] = 200; a[a.size()-1]=188;
		for(int i=0; i<aGP.m_cards.size(); ++i) 
			out << a;
			out << std::endl;
	}
	return out;
}
void GenericPlayer::endOfTurn(GenericPlayer * pl)
{
	if(this == pl || pl->isPlayAway()) return;
	STATUS temp;
	if((isBusted() && pl->isBusted()) || (getTotal() == pl->getTotal()))
		temp = end_push;
	else
	{
		if(pl->isBusted()) 
			temp = end_lose;
		else if(isBusted())
			temp = end_win;
		else
			temp = (getTotal() < pl->getTotal())?end_win:end_lose;	
		if(temp == end_lose)
		{
			++(*this);
			--(*pl);			
		}
		else
		{
			--(*this);
			++(*pl);	
		}
	}
	pl->m_status = temp;
}
//------------------------------------------------------------------------
class Player_Human : public GenericPlayer
{
public:
	Player_Human(std::string n):GenericPlayer(n){}
private:
	bool isHitting(Card * aC);	
};
bool Player_Human::isHitting(Card * aC)
{
	if(isBusted()) return false; 
	return isYes(m_name + ", do you need another card?");
}
//------------------------------------------------------------------------
class Player_AI : public GenericPlayer
{
public:
	Player_AI(std::string n):GenericPlayer(n){}
private:
	bool isHitting(Card * aC);
};
bool Player_AI::isHitting(Card * aC)
{
	int total = getTotal();	
	int house_card = aC->getValue();
	if(house_card == 1) house_card = 11;
	if(total >= 17) return false;
	if(total <= 11 ) return true;
	if(house_card >= 7) return true;
	if(total >= 15) return false;
	return (rand()%2 == 0);	//dlya intrigi
}
//------------------------------------------------------------------------
class House : public GenericPlayer
{
public:
	House():GenericPlayer("[HOUSE]"){}
	void flipFirstCard(){if(m_cards.size() != 0) m_cards[0]->flip();}
	Card * getUpCard(){return m_cards[0];}
private:
	bool isHitting(Card *){return getTotal() < 17;}
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
	int play();	
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
int Game::play()		
{	
	House * house = ((House *)m_players.back());
	int pl_aw = 0;
	for(int i=0; i<m_players.size(); ++i)
		m_players[i]->startOfTurn();
	m_deck.shuffle();	
	for(int i=0; i<m_players.size(); ++i) 
	{
		if(m_players[i]->isPlayAway())
			++pl_aw;
		else
		{
			for(int j=0; j<2; ++j)
				m_deck.deal(m_players[i]);
		}
	}
	house->flipFirstCard();
	std::cout << *this;
	if(house->isPlayAway()) return -1;
	if(m_players.size()-pl_aw == 1) return 1;
	Card * house_card = house->getUpCard();
	
	
	for(int i=0; i<m_players.size(); ++i)
	{
		while(m_players[i]->isHitting(house_card))
		{
			m_deck.deal(m_players[i]);			
			std::cout << *this;
		}
		std::cout << *this;
	}
	house->flipFirstCard();
	for(int i=0; i<m_players.size(); ++i)
		house->endOfTurn(m_players[i]);
	std::cout << *this;
	return 0;
}
//------------------------------------------------------------------------
int main()
{
	srand(time(NULL));
	system("cls");
	unsigned char ch[] = {3, 4, 5, 6, 0};
	std::cout << ch << " WELCOME TO BLACKJACK " << ch << std::endl << std::endl;  
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
		int res = aGame.play();
		if(res != 0)
		{
			std::cout << "GAME OVER. HOUSE IS " << (res < 0?"LOSE !!":"WIN !!") << std::endl;
			break;	
		}		
	} 
	while(isYes("Do you want to play next turn?"));
	return 0;
} 