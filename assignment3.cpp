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

void calculateTestAverage(double &average, vector<int> testSet){
	int sum = 0;
	int total = 0;
	for(int i = 0; i < 1000; i++){
		if(testSet.at(i) != 0){
			sum += testSet.at(i);
			total++;
		}
	}
	average = (double)sum/(double)total;
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

void calculateTestAverageDifferences(vector<double> &testAverageDifferences, double avg, vector<int> testSet){
	for(int i = 0; i < 1000; i++){
		testAverageDifferences.at(i) = (double)testSet.at(i) - avg;
	}
	return;
}

/*Calulates the similarity weight between test user and each training user*/
void calculateWeights(vector<vector<double> > averageDiff, vector<double> &weights, vector<double> testAvgDiff) {
	
	double sumOfDiffs = 0; //top of fraction in equation
	double sumOfTrainingUser = 0; //part of bottom half of equation
	double sumOfTestUser = 0; //part of bottom half of equation
	double weight = 0;
		
	for(int i = 0; i < 200; i++) {
                for(int j = 0; j < 1000; j++) {
			if((averageDiff.at(i).at(j) != 0) && testAvgDiff.at(j) != 0) { //If both users have rated movie
				sumOfDiffs += averageDiff.at(i).at(j) *	testAvgDiff.at(j); //multiply differences and add to sum
				sumOfTrainingUser += pow(averageDiff.at(i).at(j), 2); //Square differences and add to sum
				sumOfTestUser += pow(testAvgDiff.at(j), 2); //Square differences and add to sum
			}
                }
		weight = sumOfDiffs / (sqrt(sumOfTrainingUser * sumOfTestUser)); //Doing final weight calculation
		weights.at(i) = weight; //Saving weight to weight vector

		sumOfDiffs = 0; //Clearing values for next user
		sumOfTrainingUser = 0;
		sumOfTestUser = 0;
        }
	return;
}

/*Finds the most likely rating of a single object for a test user*/
int findObjectRating(vector<vector<double> > averageDiff, vector<double> weights, int desiredMovieRating, double testAvg) {

	double weightAvgDiffSum = 0;	//numerator
	double weightSum = 0;	//denominator
	int rating = 0;

	for(int i = 0; i < 200; i++) {
		weightAvgDiffSum += weights.at(i) * averageDiff.at(i).at(desiredMovieRating - 1); //Summing a user's weight times the averageDiff of object
		weightSum += abs(weights.at(i));
	}
	double temp_rating = testAvg + (weightAvgDiffSum / weightSum);
	if((ceil(temp_rating) - temp_rating) > 0.5)
		rating = floor(temp_rating);
	else
		rating = ceil(temp_rating);
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
	vector<double> weights(200); //holds weights for similarity to test user (user 200)
	int rating = 0;
	int desiredMovieRating = 0;
	double testAverage = 0;
	vector<int> testSet(1000);
	vector<double> testAverageDifferences(1000);

	/*Beginning Calculations for Part 1*/

	readTrainingSet(trainingSet); //Filling matrix with training set data
	findUserAverages(trainingSet, averageUserRating); //Finds the average rating given by each user
	findAverageDifference(trainingSet, averageUserRating, averageDiff); //Finds the difference between a user's average rating and all their ratings
	
	cout << "Enter a movie number(1-1000) to find the test user's expected rating: ";
	cin >> desiredMovieRating;
	
	for(int i = 0; i < 1000; i++){
		if(trainingSet.at(199).at(i) != 0){
			cout << "Movie " << i+1 << " = " << trainingSet.at(199).at(i) << endl;
		}
	}
	while(true) {
		if(desiredMovieRating == 0)
			break;
		testSet = trainingSet.at(199);
		testSet.at(desiredMovieRating - 1) = 0;
		calculateTestAverage(testAverage,testSet);
		calculateTestAverageDifferences(testAverageDifferences,testAverage,testSet);
		calculateWeights(averageDiff,weights,testAverageDifferences);

		rating = findObjectRating(averageDiff, weights, desiredMovieRating,testAverage);
		
		cout << "The expected rating of user 200 for movie " << desiredMovieRating << " is " << rating << endl;
		cout << "It should be " << trainingSet.at(199).at(desiredMovieRating-1) << endl;
		cout << "Enter another movie number(1-1000) to test or enter 0 to quit" << endl;
		cin >> desiredMovieRating;
		testSet.clear();
		testAverage = 0;
	}
		
	return 0;
}

