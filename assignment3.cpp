#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

/*Reads in the user data from file and saves to trainingSet matrix*/
void readTrainingSet(vector<vector<int> > &trainingSet) {

	ifstream data;
	data.open("dataset.txt");
	
	for(int i = 0; i < 200; i++) {
		for(int j = 0; j < 1000; j++) {
			data >> trainingSet.at(i).at(j); //Reads each value and stores to trainingSet matrix
		}
	}

	data.close();
	return;
}

/*Finds average rating of each user and stores to averageUserRating vector*/
void findUserAverages(vector<vector<int> > trainingSet, vector<double> &averageUserRating) {
	int sum = 0; //Sum of a row
	double ratedMovies = 0; //Number of movies user actually rated (nonzero data values)

	for(int i = 0; i < 200; i++) {
		for(int j = 0; j < 1000; j++) {
			sum += trainingSet.at(i).at(j); //Adding rating to sum
			if(trainingSet.at(i).at(j) != 0) ratedMovies++; //User actually rated this movie
		}
		averageUserRating.at(i) = (double) sum / ratedMovies; //Diving sum by number of movies and adding avg to matrix
		sum = 0;
		ratedMovies = 0;
	}

	return;
}

/*Finds the difference between each user rating and the user's average rating*/
void findAverageDifference(vector<vector<int> > trainingSet, vector<double> averageUserRating, vector<vector<double> > &averageDiff) {

	for(int i = 0; i < 200; i++) {
                for(int j = 0; j < 1000; j++) {
			if(trainingSet.at(i).at(j) != 0) { //If user has given a rating
				/*Average difference = individual rating - user's average rating*/
                        	averageDiff.at(i).at(j) = (double)trainingSet.at(i).at(j) - averageUserRating.at(i);
			}
                }
        }

        return;
}

/*Prints a vector to a file for testing purposes*/
void testVector(vector<double> v) {
	ofstream myFile;
	myFile.open("vector.txt");
	
	for(int i = 0; i < 200; i++) {
                        myFile << v.at(i) << " ";
        }

	myFile.close();
	return;
}

/*Prints a 2D vector to a file for testing purposes*/
void testMatrix(vector<vector<double> > m) {

	ofstream myFile;
	myFile.open("matrix.txt");
	
	for(int i = 0; i < 200; i++) {
                for(int j = 0; j < 1000; j++) {
			myFile << m.at(i).at(j) << "\t";
                }
		myFile << endl;
        }
	
	myFile.close();
	return;
}

int main() {
	
	/*Variable Declarations*/
	vector<vector<int> > trainingSet(200, vector<int> (1000, 0));
	vector<double> averageUserRating(200);
	vector<vector<double> > averageDiff(200, vector<double> (1000, 0));

	/*Beginning Calculations for Part 1*/

	readTrainingSet(trainingSet); //Filling matrix with training set data

	findUserAverages(trainingSet, averageUserRating); //Finds the average rating given by each user

	findAverageDifference(trainingSet, averageUserRating, averageDiff); //Finds the difference between a user's average rating and all their ratings
	
	testMatrix(averageDiff); //for testing, prints averageDiff matrix to file "matrix.txt"
	testVector(averageUserRating); //for testing, prints averageUserRating vector to file "vector.txt"
	
	return 0;
}

