#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void initArray(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = 0;
    }
}

int main(int argc, char *argv[])
{
    string sequence;
    int sequence_count[26];

    initArray(sequence_count, 26);

    if (argc < 2)
    {
        cout << "Invalid number of params." << endl;
        return 1;
    }

    ifstream file(argv[1]);

    if (file.is_open())
    {
        getline(file, sequence);
        file.close();
    }
    else
    {
        cout << "Unable to open file" << argv[1] << endl;
        return 1;
    }

    int sequence_size = sequence.length();

    int index;
    for (int i = 0; i < sequence_size; ++i)
    {
        index = sequence[i] - 'a';
        if (index < 26 && index >= 0)
        {
            sequence_count[index]++;
        }
        else
        {
            cout << "Invalid char [" << sequence[i] << "]" << endl;
        }
    }

    cout << "Sequence occurrences:";

    for (int j = 0; j < 26; ++j)
    {
        if (sequence_count[j] > 0)
        {
            cout << " " << (char)('a' + j) << "=" << sequence_count[j];
        }
    }
    cout << endl;

    return 0;
}