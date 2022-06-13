#pragma once

#include "dealer.h"

class PlayerAbstract
{
protected:
	

public:
	virtual ~PlayerAbstract() {};
	//получает признак "мой ход"
	virtual void YouTurn(bool) = 0;
	//игрок кладёт карту на стол (headTrick[0][*])
	virtual void PutCard() = 0;
	// забирает все карты со стола
	virtual void TakeCards() = 0;
	// отбивает карту (кладёт карту в (headTrick[1][*])
	virtual void GetHeadTrick() = 0;
	//взял одну карту 
	virtual void TakeOneCard(Card * &nc) = 0;
	// вывел свои карты на экран ранг (один/два символа, масть - символ)
	virtual void ShowCards() = 0;
	// возвращает истину, если на руках карт меньше 6
	virtual bool INeedCard() = 0;
	// возвращает число карт на руках
	virtual int GetCardNum() = 0;
};

struct GameState
{
	// Козырь в текущей игре
	int trumpSuit;
};

constexpr int deckSize = 52;

class Player : public PlayerAbstract
{
// Реализуйте интерфейсы абстрактного класса 
// Доступные методы из класса Dealer можно увидеть в файле dealer.h
public:
	
	Player(const char* name);
	~Player() = default;

	// см. выше
	void YouTurn(bool flag) final;
	void PutCard() final;
	void TakeCards() final;
	void GetHeadTrick() final;
	void TakeOneCard(Card * &nc) final;
	void ShowCards() final;
	bool INeedCard() final;
	int GetCardNum() final;

private:

	// Сдвигает указатели в руке влево до упора.
	void normalizeHand();
	// Проверяет, имеет ли первая карта меньший ранг, или уступает по козырности
	bool less(Card* first, Card* secnd) const;
	// Проверяет, можно ли первой картой побить вторую.
	bool canDefend(Card* first, Card* secnd) const;
	// Достаёт карту из руки
	Card * popCard(int index);
	// Выбор атакующей карты
	int chooseAttackingCard() const;
	// Выбор защищающей карты
	int chooseDefendingCard() const;

	// Имя игрока
	const char* m_name;
	// Рука с картами
	Card* m_hand[deckSize];
	// Количество карт в руке.
	int m_cardscount = 0;
	// Состояние игры
	GameState m_state;
	
};

using Player0408 = Player;
