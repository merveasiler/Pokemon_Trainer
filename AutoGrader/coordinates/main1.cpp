#include <iostream>
#include <cstdlib>

using namespace std;

int main() {

	int i, j, x, y, z;
	int coordinates[50][3];

	for(i=0; i<50; i++)
	{
		x = rand() % 7;
		y = rand() % 7;
		z = rand() % 7;

		for (j=0; j<i; j++)
		{
			if (coordinates[j][0] == x && coordinates[j][1] == y && coordinates[j][2] == z) {
				z = rand() % 7;
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
