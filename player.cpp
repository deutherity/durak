#include <iostream>
#include "dealer.h"
#include "player.h"

template <typename T>
void swap(T& a, T& b)
{
	T& tmp = a;
	a = b;
	b = tmp;
}

bool Player::less(Card* first, Card* secnd) const
{
	auto s1 = Dealer::SuitIndex(first);
	auto s2 = Dealer::SuitIndex(secnd);

	auto r1 = Dealer::RankIndex(first);
	auto r2 = Dealer::RankIndex(secnd);

	auto trumpSuit = m_state.trumpSuit;

	if (s1 != s2)
	{
		// Если масти различаются, то надо проверить козырная ли масть
		if (s1 == trumpSuit)
			return false;
		if (s2 == trumpSuit)
			return true;
		return r1 < r2;
	}
	return r1 < r2;
}


Player::Player(const char* name): m_name(name)
{
	for (int i = 0; i < deckSize; ++i)
		m_hand[i] = nullptr;
	m_state.trumpSuit = Dealer::SuitIndex(Dealer::GetTrump());
}

void Player::normalizeHand()
/*
 * Сдвигает карты влево
 * 0 0 1 0 0 2 0 3 4  --->  1 2 3 4 0 0 0 0 0
 * 1 2 3 0
 */
{
	Card * cards[deckSize];
	for (int i = 0; i < deckSize; ++i)
		cards[i] = m_hand[i];
	
	int j = 0;
	for (int i = 0; i < deckSize; ++i)
		if (cards[i] != nullptr)
			m_hand[j++] = cards[i];
	m_cardscount = j;
	for (; j < deckSize; ++j)
		m_hand[j] = nullptr;
}

void Player::YouTurn(bool flag)
{
	m_state.my_turn = true;
	if (flag)
		m_state.have_to_place_card = true;
}

int Player::chooseAttackingCard() const
// Найти индекс карты, которой хочется сходить
{
	// Найти карту наименьшего достоинства
	Card *minimal = m_hand[0];
	int minimal_index = 0;
	for (int i = 1; i < m_cardscount; ++i)
	{
		if (less(m_hand[i], minimal))
		{
			minimal = m_hand[i];
			minimal_index = i;
		}
	}
	return minimal_index;
}

constexpr int unchosen = -1;

void Player::PutCard()
// Атака
{
	if (m_cardscount == 0)
		Dealer::Attack(Dealer::GetNocard());
	int index = chooseAttackingCard();
	Card * chosenCard = m_hand[index];
	if (Dealer::GetCurrentHeadTrik() != 0)
	{
		// if (Dealer::SuitIndex(chosenCard) == m_state.trumpSuit)
		{
			chosenCard = Dealer::GetPas();
			index = unchosen;
		}
	}
	if (index != unchosen) // Значит надо убрать карту из руки и сдвинуть карты в руке влево
	{
		m_hand[index] = nullptr;
		normalizeHand();
		m_state.have_to_place_card = false;
	}
	Dealer::Attack(chosenCard);
}

void Player::TakeCards()
{
	const auto table = Dealer::GetheadTrick();
	int end = Dealer::GetCurrentHeadTrik();
	for (int i = 0; i < end; ++i)
		for (int j = 0; j < 2; ++j)
		{
			Card *card = table[j][i];
			if (card != nullptr &&
				Dealer::RankIndex(card) != PAS &&
				Dealer::RankIndex(card) != NOCARD)
				m_hand[m_cardscount++] = table[j][i];
		}
}

void Player::GetHeadTrick()
// Защита
{
	auto rank = Dealer::RankIndex(Dealer::GetLastCard());
	if (rank == PAS || rank == NOCARD)
		return Dealer::Defend(Dealer::GetNocard());
	if (m_cardscount == 0)
		return Dealer::Defend(Dealer::GetNocard());
	int index = chooseDefendingCard();
	if (index == unchosen) // Если нечем крыть - пасуем
		return Dealer::Defend(Dealer::GetPas());
	// Если есть чем - кроем
	// Надо убрать карту из руки и сдвинуть карты в руке
	Card* chosenCard = m_hand[index];
	m_hand[index] = nullptr;
	normalizeHand();
	Dealer::Defend(chosenCard);
}

bool Player::canDefend(Card* first, Card* secnd) const
{
	auto s1 = Dealer::SuitIndex(first);
	auto s2 = Dealer::SuitIndex(secnd);

	return less(secnd, first) && (s1 == s2 || s1 == m_state.trumpSuit);
}

int Player::chooseDefendingCard() const 
{
	Card *attackingCard = Dealer::GetLastCard(); // Карта, которую нужно покрыть
	bool use_trump = Dealer::getcurrentCard() == deckSize; // если конец игры, то используй козыри
	int minimal_index = unchosen; // Индекс карты, которой мы хотим покрыть
	
	for (int i = 0; i < m_cardscount; ++i)
	{
		Card* card = m_hand[i];
		bool is_trump_card = Dealer::SuitIndex(card) == m_state.trumpSuit;
		if (canDefend(card, attackingCard) &&
			(!is_trump_card || use_trump)) // Если такой картой можно защититься
										   // и если это не козырь или мы можем использовать козыри
		{
			if (minimal_index == unchosen) // Если ещё не выбрали карту
				minimal_index = i;
			else if (less(card, m_hand[minimal_index])) // или нашли карту поменьше
				minimal_index = i;
			// то выбираем эту карту
		}
	}
	return minimal_index;
}


void Player::TakeOneCard(Card*&nc)
{
	m_hand[m_cardscount++] = nc;
}

void Player::ShowCards()
{
	for (int i = 0; i < m_cardscount; ++i)
	{
		Card * card = m_hand[i];
		std::cout << Dealer::RankName(card)[0] <<
#ifdef _WIN32
		suitsSymb[Dealer::SuitIndex(card)]
#else
		Dealer::SuitName(card)[0]
#endif
		 << ' ';
	}
	std::cout << std::endl;
}

bool Player::INeedCard()
{
	return m_cardscount < 6 && Dealer::getcurrentCard() < 52;
}

int Player::GetCardNum()
{
	return m_cardscount;
}
