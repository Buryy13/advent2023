#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include <set>

using std::cout;
using std::endl;

bool checkSubString(const std::string& str, size_t pos, size_t len)
{ //...453.
  //0123456
  size_t indx = pos;
  if(indx > 0) indx--;
  size_t last = pos+len-1;
  if(last < str.size()-1) last++;
  char c;
  do{
    c=str[indx];
    if(!std::isdigit(c) && c != '.')
      return true;
  } while(++indx<=last);
  return false;
}

bool checkGearTopAndBotStrings(std::string str, size_t i)
{
  return std::isdigit(str[i-1]) || std::isdigit(str[i]) || std::isdigit(str[i+1]);
}

struct number
{
  number(const std::string& s, size_t p, size_t l) : 
    str(s),
    stringNumber(str, p, l),
    pos(p),
    len(l),
    num(std::stoi(stringNumber)) {}

  bool checkIndxTopAndBot(size_t indx) const
  {
    // cout << "POS: " << pos << " lastIndx: " << lastIndx << " indx: " << indx << endl;
    return ((pos <= indx-1) && (indx-1 <= lastIndx)) || ((pos <= indx) && (indx <= lastIndx)) || ((pos <= indx+1) && (indx+1 <= lastIndx));
  }
  bool checkIndxLeft(size_t indx) const
  {
    return lastIndx == indx-1;
  }
  bool checkIndxRight(size_t indx) const
  {
    return pos == indx+1;
  }

  const std::string& str;
  std::string stringNumber;
  size_t pos;
  size_t len;
  int num;
  bool valid = false;
  size_t lastIndx = pos+len-1;
};

struct String
{
  String(const std::string& s) : str(s) {}
  String& operator=(String& rhs) { 
    str = rhs.str;
    numbers.clear();
    for(auto& n : rhs.numbers) numbers.push_back(n);
    stars.clear();
    for(auto& i : rhs.stars) stars.insert(i);
    return *this;
  }

  std::string str;
  const size_t lastIndx = str.size()-1;
  std::vector<number> numbers;
  std::set<int> stars;

  void getNumbers()
  {
    auto first = str.find_first_of("123456789");
    while(first != std::string::npos)
    {
      auto last = str.find_first_not_of("0123456789", first+1);
      if(last == std::string::npos)
      { 
        numbers.emplace_back(str, first, str.size()-first);
        break;
      }
      else 
        numbers.emplace_back(str, first, last-first);

      if (last < str.size()-1)
        first = str.find_first_of("123456789", last+1);
      else
        break;
    }
  }
  void getStars()
  {
    auto indx = str.find_first_of("*");
    // cout << "STARS: ";
    while(indx != std::string::npos)
    {
      stars.insert(indx);
      // cout << indx << " ";
      indx = str.find_first_of("*", indx+1);
    }
    // cout << endl;
  }
  long long getGearsSingleString()
  {
    long long gear = 0;
    for(auto& i : stars)
    {
      // left&right
      if(i > 0 && i < lastIndx && std::isdigit(str[i-1]) && std::isdigit(str[i+1])){
        gear += findNumberLeft(*this, i)*findNumberRight(*this, i);
        cout << "ADD GEAR LEFT&LEFT!" << endl;
      }
    }
    cout << "GEAR: " << gear << endl;
    return gear;
  }
  long long getGears(String& top, String& bot)
  {
    long long gear = 0;
    for(auto& i : stars)
    {
      cout << "TOP: " << top.str << endl;
      cout << "MID: " << str << endl;
      cout << "BOT: " << bot.str << endl;
      // ..123*
      // 012345
      // if i == 0 then check top&bot + right&top + right&bot
      // if i == lastIndx check top&bot + left&top + left&bot 
      
      // top&top
      if((std::isdigit(top.str[i-1])) && (!std::isdigit(top.str[i])) && (std::isdigit(top.str[i+1]))){
        cout << "ADD GEAR TOP&TOP!" << endl;
        gear += findNumberTopAndBot(top, i-1)*findNumberTopAndBot(top, i+1);
        continue;
      }

      // bot&bot
      if(std::isdigit(bot.str[i-1]) && !std::isdigit(bot.str[i]) && std::isdigit(bot.str[i+1])){
        cout << "ADD GEAR BOT&BOT!" << endl;
        gear += findNumberTopAndBot(bot, i-1)*findNumberTopAndBot(bot, i+1);
        continue;
      }

      // left&right
      if(std::isdigit(str[i-1]) && std::isdigit(str[i+1])){
        cout << "ADD GEAR LEFT&RIGHT!" << endl;
        gear += findNumberLeft(*this, i)*findNumberRight(*this, i);
        continue;
      }

      // top&bot
      // if(checkG/ear(top.str, i) && checkGear(bot.str, i)){
      if(checkGearTopAndBotStrings(top.str, i) && checkGearTopAndBotStrings(bot.str, i)){
        cout << "ADD GEAR TOP&BOT!" << endl;
        gear += findNumberTopAndBot(top, i)*findNumberTopAndBot(bot, i);
        continue;
      }
      
      // left&top + left&bot
      if(std::isdigit(str[i-1])){
        if(checkGearTopAndBotStrings(top.str, i)){
        cout << "ADD GEAR LEFT&TOP!" << endl;
          gear += findNumberLeft(*this, i)*findNumberTopAndBot(top, i);
          continue;
        }
        else if(checkGearTopAndBotStrings(bot.str, i)){
        cout << "ADD GEAR LEFT&BOT!" << endl;
          gear += findNumberLeft(*this, i)*findNumberTopAndBot(bot, i);
          continue;
        }
      }

      // right&top + right&bot
      if(std::isdigit(str[i+1])){
        if(checkGearTopAndBotStrings(top.str, i)){
        cout << "ADD GEAR RIGHT&TOP!" << endl;
          gear += findNumberRight(*this, i)*findNumberTopAndBot(top, i);
          continue;
        }
        else if(checkGearTopAndBotStrings(bot.str, i)){
        cout << "ADD GEAR RIGHT&BOT!" << endl;
          gear += findNumberRight(*this, i)*findNumberTopAndBot(bot, i);
          continue;
        }
      }
    }
    cout << "Gear: " << gear << endl;
    return gear;
  }

  int findNumberTopAndBot(const String& strObj, size_t indx)
  {
    for(const auto& n : strObj.numbers)
      if(n.checkIndxTopAndBot(indx)) return n.num;
    return 0;
  }
  int findNumberLeft(const String& strObj, size_t indx)
  {
    for(const auto& n : strObj.numbers)
      if(n.checkIndxLeft(indx)) return n.num;
    return 0;
  }
  int findNumberRight(const String& strObj, size_t indx)
  {
    for(const auto& n : strObj.numbers)
      if(n.checkIndxRight(indx)) return n.num;
    return 0;
  }

  void validateString(String& top, String& bot)
  {
    cout << "top str: " << top.str << endl;
    cout << "mid str: " << str << endl;
    cout << "bot str: " << bot.str << endl;
    for(auto& n : numbers)
    {
      if(n.valid = checkSubString(str, n.pos, n.len)) { cout << "M: valid number spoted: " << n.num << endl; continue; }
      if(n.valid = checkSubString(top.str, n.pos, n.len)) { cout << "T: valid number spoted: " << n.num << endl; continue; }
      if(n.valid = checkSubString(bot.str, n.pos, n.len)) { cout << "B: valid number spoted: " << n.num << endl; continue; }
    }
  }
  int getSumOfValidNumbers()
  {
    int sum = 0;
    for(auto& n : numbers) if(n.valid) sum+=n.num;
    cout << "Adding sum of valid numbers: " << sum << endl;
    return sum;
  }
};

int main()
{
  std::ifstream file;
  file.open("engine.txt");
  std::string str;
  long long sum = 0;
  int gear;
  std::getline(file, str);
  String top(str);
  top.getNumbers();
  top.getStars();
  sum += top.getGearsSingleString();
  
  std::getline(file, str);
  String mid(str);
  mid.getNumbers();

  // top.validateString(top, mid);
  // sum += top.getSumOfValidNumbers();
  
  while(std::getline(file, str))
  {
    String bot(str);
    bot.getNumbers();
    // mid.validateString(top, bot);
    mid.getStars();
    sum += mid.getGears(top, bot);
    // sum += mid.getSumOfValidNumbers();

    top = mid;
    mid = bot;
   // break;
  }
  // mid.validateString(top, mid);
  mid.getStars();
  sum += mid.getGearsSingleString();
  // sum += mid.getSumOfValidNumbers();
  
  cout << "Result: " << sum << endl;
  return 0;
}
