#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    string test_vector, test_text, matrix_row, text_row, most_similar_text;

    if (argc < 3)
    {
        cout << "Invalid number of params. Usage:" << endl;
        cout << "jaccard_similarity TEXT_FILE MATRIX_FILE" << endl;
        return 1;
    }

    ifstream text_file(argv[1]);

    if (!text_file.is_open())
    {
        cout << "Unable to open text file: " << argv[1] << endl;
        return 1;
    }

    ifstream matrix_file(argv[2]);
    if (!matrix_file.is_open())
    {
        cout << "Unable to open matrix file" << argv[2] << endl;
        text_file.close();
        return 1;
    }

    // The first line is expected to be the text to be evaluated against the rest.
    getline(matrix_file, test_vector); // Get the vector representation of the text to be evaluated
    getline(text_file, test_text); // Get the text to be evaluated

    double words_union, words_intersection, jaccard_similarity, max_jaccard_similarity;;
    max_jaccard_similarity = -1.0;
    int i;
    
    while (getline(matrix_file, matrix_row) && getline(text_file, text_row)) {
        words_union = words_intersection = 0.0;
        for (i = 0; i < matrix_row.length(); ++i) {
            if (matrix_row[i] == '1' || test_vector[i] == '1') { // If the word is present in any of the texts being compared, sum up to Union
                ++words_union;
            }
            if (matrix_row[i] == '1' && test_vector[i] == '1') { // If the word is present in both texts, then sum up to Intersection
                ++words_intersection;
            }
        }
        jaccard_similarity = words_intersection / words_union; // Calculate Jaccard similarity score

        if (jaccard_similarity > max_jaccard_similarity) { // Update highest Jaccard score if it is greater than the previous one
            max_jaccard_similarity = jaccard_similarity;
            most_similar_text = text_row;
        }
    }
    matrix_file.close();
    text_file.close();
    cout << "\nEvaluated text: \n\"" << test_text << "\"" << endl;
    cout << "\nMost similar text: \n\"" << most_similar_text << "\"" << endl;
    printf("\nJaccard similarity score: %.6f\n", max_jaccard_similarity);

    return 0;
}
