#include <iostream>
#include <unordered_map>

using std::cout;
using std::endl;
using ull = unsigned long long;

int main()
{
  // part 2
  const ull timeLimit = 48938595;
  const ull distRecord = 296192812361391;
  ull waysToWin = 0;
  for(ull i = 1; i <= timeLimit/2; ++i)
  {
    ull dist = i*(timeLimit-i);
    if(dist > distRecord) waysToWin++;
  }
  waysToWin*=2;
  if(timeLimit % 2 == 0) waysToWin--;
  cout << waysToWin << endl;

  // part 1
  // int res = 1;
  // const std::unordered_map<int, int> time2dist = {
  //   // {48, 296},
  //   // {93, 1928},
  //   // {85, 1236},
  //   // {95, 1391}
  //   {7, 9},
  //   {15, 40},
  //   {30, 200}
  // };

  // for(const auto& [timeLimit, distRecord] : time2dist)
  // {
  //   int waysToWin = 0;
  //   for(int i = 1; i <= timeLimit/2; ++i)
  //   {
  //     int dist = i*(timeLimit-i);
  //     if(dist > distRecord) waysToWin++;
  //   }
  //   waysToWin*=2;
  //   if(timeLimit % 2 == 0) waysToWin--;
  //   res *= waysToWin;
  //   cout << "waysToMin: " << waysToWin << endl;
  // }
  // cout << res << endl;
  return 0;
}