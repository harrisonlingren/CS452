#include <iostream>
#include <fstream>

using namespace std;

int main() {

  char letters[1000];
  ifstream inputf("letters.txt");
  inputf >> letters;

  int counts[256] = { 0 };

  for (int i = 0; i < 1000; i++) {
      counts[(int)(letters[i])]++;
  }
  for (int i = 0; i < 256; i++) {
      printf("The %d. character has %d occurrences.\n", i, counts[i]);
  }
}
