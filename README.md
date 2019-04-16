# Assignment3

Part One: Implementing a memory-based Collaborative Filtering Algorithm
    + Algorithm
      1. Store training set in matrix.
      2. Find the average rating for each user.
      3. Find the difference between a user's average rating and each individual rating.
      4. Enter the movie number you want the expected rating of.
      5. Calculate the average of the test set and the average differences of the test set.
      6. Calculate the similarity weights for each user.
            The similarity weight is determined by adding the sum of each user's difference times the differences of the test user. This value is divided by the product of the square roots of the sum of the squares of each user and the test user.
      7. Calculate the expected rating.
            The expected rating is calculated by adding the average user rating to the sum of a user's weight times the average difference and divided by the sum of the absolute value of the weights.
            
    + Design Considerations
      - User 200 was considered as the test user.
      
      
Part Two: Implement our own Collaborative Filtering Algorithm
    + Algorithm
      1. Reuse previously calculated average differences and user averages
      2. Calculate the item weights using the Adjusted Cosine Similarity formula
          The similarity weight is determined by the sum of the product of the differences between each movie's rating and the user's average rating. This value is divided by the product of the square roots of the sum of the squares of the same differences.
      3. Use Case Adjustment to transform the weights
      4. Calculate the expected rating
          The expected rating is calculated with the Weighted Sum algorithm by finding the sum of the item weight times its rating divided by the sum of the absolute value of the similar weights.
          
    + Design Considerations
      - We implemented an item-based Collaborative Filtering Algorithm using Adjusted Cosine-Based Similarity, Case Adjustment, and Weighted Sum.
      - In this algorithm, the test set is a movie instead of a user.
      - Movie 1 was considered as the test movie.
      - A "similar weight" was defined as a weight greater than 0.
