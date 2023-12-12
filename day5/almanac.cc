#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <tuple>
#include <vector>
#include <thread>
#include <future>

using std::cout;
using std::endl;
using ull = long long;
using Numbers = std::vector<ull>;
struct DSL
{
  DSL(ull d, ull s, ull l) : dest(d), source(s), len(l) {}
  ull dest;
  ull source;
  ull len;
};

struct FilterMap
{
  FilterMap(ull s) : seed(s) {}

  ull seed;
  ull soil=-1;
  ull fert=-1;
  ull water=-1;
  ull light=-1;
  ull temp=-1;
  ull humidity=-1;
  ull location=-1;
};
std::mutex mu;
Numbers getNumbersFromString(const std::string& str)
{
  // 83 86  6 
  // 012345678
  Numbers numbers;
  size_t delim = str.find_first_of(' ');
  size_t first = 0;
  while(delim != std::string::npos)
  {
    std::string strNum = str.substr(first, delim-first);
    if(strNum.size())
      numbers.push_back(std::stoll(strNum));
    first = delim+1;
    delim = str.find_first_of(' ', first);
  }
  numbers.push_back(std::stoll(str.substr(first, delim)));
  return numbers;
}
Numbers getSeeds(std::ifstream& file, std::string str)
{
  Numbers seeds;
  std::getline(file, str);
  str = str.substr(str.find_first_of("123456789"));
  for(const auto& n : getNumbersFromString(str)) seeds.push_back(n);
  return seeds;
}
std::map<ull, ull> getSeedRanges(const Numbers& seeds)
{
  std::map<ull, ull> ranges;
  for(auto itr = seeds.begin(), itrRange = std::next(itr); itr != seeds.end(); itr+=2, itrRange+=2)
    ranges[*itr] = *itrRange;
  return ranges;
}
std::vector<DSL> getDSL(std::ifstream& file, std::string str)
{
  std::vector<DSL> vec;
  std::getline(file, str); // read caption string
  while(std::getline(file, str) && str.size())
  {
    Numbers numbers = getNumbersFromString(str);
    vec.emplace_back(DSL(numbers[0], numbers[1], numbers[2]));
  }
  return vec;
}
void transfer2Next(const std::vector<DSL>& dslVec, ull cur, ull& next)
{
  for(auto dsl : dslVec)
    if(cur >= dsl.source && cur < dsl.source+dsl.len){
        next = dsl.dest-dsl.source+cur;
        return;
  }
  if(next == -1) next = cur;
}
ull processSeedRange(const std::vector<std::vector<DSL>>& filters, const Numbers& seeds, uint16_t from, uint16_t to)
{
  ull minLocation = 9999999999;
  for(uint16_t i = from; i < to; i+=2)
  {
    for(ull j = 0; j < seeds[i+1]; ++j)
    {
      FilterMap filterMap(seeds[i]+j);

      transfer2Next(filters[0], filterMap.seed, filterMap.soil);
      transfer2Next(filters[1], filterMap.soil, filterMap.fert);
      transfer2Next(filters[2], filterMap.fert, filterMap.water);
      transfer2Next(filters[3], filterMap.water, filterMap.light);
      transfer2Next(filters[4], filterMap.light, filterMap.temp);
      transfer2Next(filters[5], filterMap.temp, filterMap.humidity);
      transfer2Next(filters[6], filterMap.humidity, filterMap.location);

      if(filterMap.location < minLocation) minLocation = filterMap.location;
    }
      mu.lock();
      cout << seeds[i+1] << " seeds have been processed. To:" << to << endl;
      mu.unlock();
  }
  return minLocation;
}

int main()
{
  std::ifstream file;
  file.open("almanac.txt");
  std::string str;
  Numbers seeds = getSeeds(file, str);
  // std::map<ull, ull> ranges = getSeedRanges(seeds);

  // split the biggest range into 2
  // auto maxRange = ranges.begin();
  // for(auto it = std::next(maxRange); it != ranges.end(); ++it)
  //   if(it->second > maxRange->second) maxRange = it;

  // auto halfRange = maxRange->second/2;
  // ull newRangeStart = maxRange->first+halfRange;

  // ull newRangeLen;
  // if (maxRange->second%2 == 0)
  //   newRangeLen = halfRange;
  // else
  //   newRangeLen = halfRange+1;
  // ranges[newRangeStart]=newRangeLen;
  // maxRange->second = halfRange;

  // transform ranges into seeds
  // seeds.clear();
  // for(auto r : ranges) {seeds.push_back(r.first); seeds.push_back(r.second); }

  ull totalAmountOfSeeds = 0;
  for(uint16_t i = 1; i < seeds.size(); i+=2)
    totalAmountOfSeeds+=seeds[i];

  cout << "Total amount of input seeds: " << totalAmountOfSeeds << endl;
  std::getline(file, str); // read empty string
  ull minLocation = 9999999999;
  std::vector<std::vector<DSL>> filters;
  filters.reserve(7);
  for(int i = 0; i < 7; ++i)
    filters.push_back(getDSL(file, str));

  std::vector<ull> futureMinLocations;
  // test execution
  // futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 0, 2).get());
  // futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 2, 4).get());
  // futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 4, 6).get());

  // final execution
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 0, 6).get());
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 6, 8).get());
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 8, 12).get());
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 12, 14).get());
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 14, 16).get());
  futureMinLocations.emplace_back(std::async(processSeedRange, std::ref(filters), std::ref(seeds), 16, 20).get());

  for(auto min : futureMinLocations)
    if(min < minLocation) minLocation = min;

  // ull processedSeeds = 0;
  // for(uint16_t i = 0; i < seeds.size(); i+=2)
  // {
  //   for(ull j = 0; j < 1; ++j)
  //   {
  //     FilterMap filterMap(seeds[i]+j);

  //     transfer2Next(filters[0], filterMap.seed, filterMap.soil);
  //     transfer2Next(filters[1], filterMap.soil, filterMap.fert);
  //     transfer2Next(filters[2], filterMap.fert, filterMap.water);
  //     transfer2Next(filters[3], filterMap.water, filterMap.light);
  //     transfer2Next(filters[4], filterMap.light, filterMap.temp);
  //     transfer2Next(filters[5], filterMap.temp, filterMap.humidity);
  //     transfer2Next(filters[6], filterMap.humidity, filterMap.location);

  //     if(filterMap.location < minLocation) minLocation = filterMap.location;
  //   }
  //   processedSeeds += seeds[i+1];
  //   cout << "Processed seeds: " << processedSeeds << endl;
  //   cout << "Progress: " << (double)processedSeeds/totalAmountOfSeeds*100 << " \% of seeds have been processed." << endl;
  // }

  // part 1
  // for(const auto& s : seeds)
  //   seed2LocationMap.emplace_back(Seed2Location(s, -1, -1, -1, -1, -1, -1, -1));

  // std::getline(file, str); // read empty string

  // for(auto& e : seed2LocationMap)
  // {
  //   auto seed = std::get<0>(e);
  //   auto& soil = std::get<1>(e);
  //   auto& fert = std::get<2>(e);
  //   auto& water = std::get<3>(e);
  //   auto& light = std::get<4>(e);
  //   auto& temp = std::get<5>(e);
  //   auto& humidity = std::get<6>(e);
  //   auto& location = std::get<7>(e);

  //   transfer2Next(allDest2Sources[0], seed, soil);
  //   transfer2Next(allDest2Sources[1], soil, fert);
  //   transfer2Next(allDest2Sources[2], fert, water);
  //   transfer2Next(allDest2Sources[3], water, light);
  //   transfer2Next(allDest2Sources[4], light, temp);
  //   transfer2Next(allDest2Sources[5], temp, humidity);
  //   transfer2Next(allDest2Sources[6], humidity, location);
  // }
  // ull min = std::get<0>(seed2LocationMap[0]);
  // for(auto& e : seed2LocationMap){
  //   if(std::get<7>(e) < min)
  //     min = std::get<7>(e);
  //   cout << " Seed: " << std::get<0>(e)
  //        << " Soil: " << std::get<1>(e) 
  //        << " Fert: " << std::get<2>(e) 
  //        << " Water: " << std::get<3>(e) 
  //        << " Light: " << std::get<4>(e) 
  //        << " Temp: " << std::get<5>(e) 
  //        << " Humidity: " << std::get<6>(e) 
  //        << " Location: " << std::get<7>(e) << endl;
  // }
  cout << "Min location: " << minLocation << endl;

  return 0;
}