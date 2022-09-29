#include <random>
#include <ctime>
#include <iostream>

using namespace std;

int main() {
    srand(time(NULL));
    int num;
  
    for (int i = 0; i < 3; ++i) {
        num = random() % 5;
        cout << num << endl;
    }

    abort();
    return 0;
}
