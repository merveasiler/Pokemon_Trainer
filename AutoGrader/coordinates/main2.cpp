#include <iostream>
#include <cstdlib>

using namespace std;

int main() {

	int i, j, x, y, z;
	int coordinates[1000][3];

	for(i=0; i<1000; i++)
	{
		x = rand() % 128;
		y = rand() % 128;
		z = rand() % 128;

		for (j=0; j<i; j++)
		{
			if (coordinates[j][0] == x && coordinates[j][1] == y && coordinates[j][2] == z) {
				z = rand() % 128;
				j = 0;
			}
		}
			
		coordinates[i][0] = x;
		coordinates[i][1] = y;
		coordinates[i][2] = z;
		cout << x << " " << y << " " << z << endl;
	}

	return 0;
}
