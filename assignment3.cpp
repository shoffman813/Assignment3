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

/*Calulates the similarity weight between test user and each training user*/
void calculateWeights(vector<vector<double> > averageDiff, vector<double> &weights) {
	
	double sumOfDiffs = 0; //top of fraction in equation
	double sumOfTrainingUser = 0; //part of bottom half of equation
	double sumOfTestUser = 0; //part of bottom half of equation
	double weight = 0;
	vector<double> testUser = averageDiff.at(199); //vector for average differences of test user
	
	for(int i = 0; i < 199; i++) {
                for(int j = 0; j < 1000; j++) {
			if((averageDiff.at(i).at(j) != 0) && (testUser.at(j) != 0)) { //If both users have rated movie
				sumOfDiffs += averageDiff.at(i).at(j) *	testUser.at(j); //multiply differences and add to sum
				sumOfTrainingUser += pow(averageDiff.at(i).at(j), 2); //Square differences and add to sum
				sumOfTestUser += pow(testUser.at(j), 2); //Square differences and add to sum
			}
                }
		sumOfTrainingUser = sqrt(sumOfTrainingUser); //Taking square root of calculated sum
		sumOfTestUser = sqrt(sumOfTestUser); //Taking square root of calculated sum
		weight = sumOfDiffs / (sumOfTrainingUser * sumOfTestUser); //Doing final weight calculation
		weights.at(i) = weight; //Saving weight to weight vector

		sumOfDiffs = 0; //Clearing values for next user
		sumOfTrainingUser = 0;
		sumOfTestUser = 0;
		
        }
	return;
}

/*Finds the most likely rating of a single object for a test user*/
int findObjectRating(double testUserAverage, vector<vector<double> > averageDiff, vector<double> weights, int desiredMovieRating) {

	double weightAvgDiffSum = 0;
	double weightSum = 0;
	int rating = 0;

	for(int i = 0; i < 199; i++) {
		weightAvgDiffSum += weights.at(i) * averageDiff.at(i).at(desiredMovieRating - 1); //Summing a user's weight times the averageDiff of object
		weightSum += abs(weights.at(i));
	}
	cout << "rating is " << testUserAverage + (weightAvgDiffSum / weightSum) << endl;
	rating = ceil(testUserAverage + (weightAvgDiffSum / weightSum));
	return rating;
}

/*Prints a vector to a file for testing purposes*/
void testVector(vector<double> v) {
	ofstream myFile;
	myFile.open("vector.txt");
	
	for(int i = 0; i < v.size(); i++) {
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
	vector<double> weights(199); //holds weights for similarity to test user (user 200)
	int rating = 0;
	int desiredMovieRating = 0;

	/*Beginning Calculations for Part 1*/

	readTrainingSet(trainingSet); //Filling matrix with training set data
	findUserAverages(trainingSet, averageUserRating); //Finds the average rating given by each user
	findAverageDifference(trainingSet, averageUserRating, averageDiff); //Finds the difference between a user's average rating and all their ratings
	calculateWeights(averageDiff, weights); //Finds the weight for similarity of test user to every other user
	
	cout << "Enter a movie number(1-1000) to find the test user's expected rating: ";
	cin >> desiredMovieRating;

	while(true) {
		if(desiredMovieRating == 0) break;

		rating = findObjectRating(averageUserRating.at(199), averageDiff, weights, desiredMovieRating);

		cout << "The expected rating of user 200 for movie " << desiredMovieRating << " is " << rating << endl;
		cout << "Enter another movie number(1-1000) to test or enter 0 to quit" << endl;
		cin >> desiredMovieRating;
	}
		
	return 0;
}

