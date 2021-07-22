#include <iostream>
#include <ctime>
enum SUIT{hearts = 3, diamonds, clubs, spades};
enum VALUE{value_2 = 2, value_3, value_4, value_5, value_6, value_7, value_8, value_9, value_10, value_J = 10, value_Q = 10, value_K = 10, value_A, value_a = 1};
class Card
{
private:
	SUIT suit;
	VALUE value;
	bool show;
public:
	Card(SUIT s, VALUE v, bool sh = true):suit(s), value(v), show(sh){}
	void flip() {show = !show;}
	VALUE getValue(){return value;}
	
//	friend int main();	
};
int main()
{
//	Card cd(hearts, value_A);
//	std::cout << (char)cd.suit << cd.value << "   " << (cd.show?"TRUE":"FALSE") << std::endl;

	return 0;
} 