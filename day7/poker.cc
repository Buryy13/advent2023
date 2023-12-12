#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using ull = unsigned long long;

static const std::vector<char> cards = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
static const std::vector<char> cardsWithJoker = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};

bool compareHands(const std::string& a, const std::string& b)
{
  auto ia = std::find(cardsWithJoker.begin(), cardsWithJoker.end(), a[0]);
  auto ib = std::find(cardsWithJoker.begin(), cardsWithJoker.end(), b[0]);
  if(ia == ib) return compareHands(a.substr(1), b.substr(1));
  else return ia < ib;
}
inline std::pair<std::string, int> getHandAndBid(const std::string& str)
{
  return {str.substr(0, 5), std::stoi(str.substr(6))};
}

struct Hand
{
  Hand(std::string h) : hand(h)
  {
    fillCardsMap();
    upgradeJokers();
    rank = getRank();
  }

  void fillCardsMap(){
    for(auto c : hand){
      if(cardsToAmount.count(c)) cardsToAmount[c]++;
      else cardsToAmount[c] = 1;
    }
  }

  void upgradeJokers(){
    auto itrJoker = cardsToAmount.find('J');
    if(itrJoker == cardsToAmount.end()) return; // no jokers on hand
    if(itrJoker->second == 5) return; // all jokers on hand

    // transform jokers into cards that have the most appearances
    // 1. find the card that has the most appearances
    // IMPORTANT ignore jokers
    auto itrTheMostApearanceCard = cardsToAmount.begin();
    if(itrTheMostApearanceCard == itrJoker) itrTheMostApearanceCard++;
    for(auto itr = std::next(itrTheMostApearanceCard); itr != cardsToAmount.end(); itr++)
    {
      if(itr == itrJoker) continue;
      if(itr->second > itrTheMostApearanceCard->second)
        itrTheMostApearanceCard = itr;
    }

    // 2. increase amount of the most appearanced card by the amount of jokers
    itrTheMostApearanceCard->second += itrJoker->second;
    cardsToAmount.erase(itrJoker);
  }

  uint16_t getRank()
  {
    if(cardsToAmount.size() == 1) return 6; // five
    if(cardsToAmount.size() == 4) return 1; // pair
    if(cardsToAmount.size() == 5) return 0; // high card

    if(cardsToAmount.size() == 2) // kare || full house
    {
      uint16_t rank = 4; // rank of full house
      for(auto [c,a] : cardsToAmount)
        if(a == 4)
          rank = 5;
      return rank;
    }
    if(cardsToAmount.size() == 3) // set || 2 pairs
    {
      uint16_t rank = 2; // rank of 2 pairs
      for(auto [c,a] : cardsToAmount)
        if(a == 3)
          rank = 3;
      return rank;
    }
  }

  std::string hand;
  uint16_t rank;
  std::unordered_map<char, uint16_t> cardsToAmount;
};

class HandCmp
{
public:
  bool operator()(const Hand& lhs, const Hand& rhs) const
  {
    if(lhs.rank == rhs.rank)
      return compareHands(lhs.hand, rhs.hand);
    else
      return lhs.rank < rhs.rank;
  }
};

int main()
{
  std::map<Hand, int, HandCmp> handBids;
  std::ifstream file;
  file.open("poker.txt");
  std::string str;
  ull res = 0;
  while(std::getline(file, str))
  {
    auto [hand, bid] = getHandAndBid(str);
    Hand h(hand);
    handBids[h] = bid;
  }
  int i = 1;
  for(auto itr = handBids.begin(); itr != handBids.end(); itr++)
  {
    cout << "Hand: " << itr->first.hand << " Bid: " << itr->second << " Rank: " << i << endl;
    res += itr->second*i++;
  }
  
  cout << "RESULT: " << res << endl;
  return 0;
}