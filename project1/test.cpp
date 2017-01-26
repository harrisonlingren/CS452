#include <iostream>
#include <fstream>

using namespace std;

int main() {

  char letters[10000];
  ifstream inputf("letters.txt");
  inputf >> letters;

  int counts[26] = { 0 };

  for (int i = 0; i < 10000; i++) {
      counts[(int)(letters[i]-97)]++;
  }
  for (size_t i = 0; i < 26; i++) {
    cout << (char) (i+97) << ": " << counts[i] << endl;
  }
}
