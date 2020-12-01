#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mpi.h"
#include <cmath>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    int my_id, num_processes;
    int message_received, message_sent;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
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

    double words_union, words_intersection, jaccard_similarity, max_jaccard_similarity;
    jaccard_similarity = -1.0;
    max_jaccard_similarity = -1.0;
    int i;
    
    double largest_jaccard_found=0.000;
    int segment_size;
    int imprinter=0;
    int line_size =0;

    int my_turn = 0;
    int iterations =0;
    while (getline(matrix_file, matrix_row) && getline(text_file, text_row)) {
        words_union = words_intersection = 0.0;
        line_size = matrix_row.length();

        if(my_id == my_turn){
            for (i = 0; i < line_size; ++i) {
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
        iterations++;
        my_turn = iterations%(num_processes);
    }
    MPI_Allreduce(&max_jaccard_similarity, &largest_jaccard_found, 1, MPI_DOUBLE , MPI_MAX , MPI_COMM_WORLD);

    if(max_jaccard_similarity >= largest_jaccard_found){
        imprinter =1;
    }
    matrix_file.close();
    text_file.close();

    for(int i = 0; i<num_processes; i++){
        if( i == my_id && imprinter){
            cout << "I'm process " << my_id << ". And I found the largest jaccard" << endl;
            cout << "\nEvaluated text: \n\"" << test_text << "\"" << endl;
            cout << "\nMost similar text: \n\"" << most_similar_text << "\"" << endl;
            printf("\nJaccard similarity score: %.6f\n", max_jaccard_similarity);
            printf("\n======================================================================================\n");

        }else{
            sleep(0.1);
        }
    }
    
    MPI_Finalize();
    return 0;
}
