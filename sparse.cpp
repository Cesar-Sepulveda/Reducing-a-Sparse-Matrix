/*******************************************************************************
Cesar Sepulveda
CPSC 474-01 Project 2: Implement a distributed algorithm using MPI
12/10/21
*******************************************************************************/
#include <iostream>
#include <mpi.h>
#include <vector>
#include <fstream>

using namespace std;

//Function to read the input matrix from a file and place it in a 2 dimensional
//vector.
vector<vector<int>> readInput(int n, int m){
  vector<vector<int>> vec;
  int i = 0;                          //Initializing variables.
  int j = 0;
  ifstream file;
  file.open("input.txt");             //Opening the input file.
  int num;

  while(file >> num){                 //While loop to read from the file one input at a time.
    vec.push_back(vector<int>());  //Adding an int vector(row) to the vector of string vectors.
    vec[i].push_back(num);            //Adding the next value of the input matrix to the correct vector.
    j++;                              //Incrementing the column.
    if(j == m){                       //Condition to find the end of the line and switch rows.
      j = 0;                          //Resetting the column variable and incrementing the row variable.
      i++;
    }
    if(i == n){                       //Making sure the vector has the correct amount of rows.
      vec.resize(n);
      break;
    }
  }

  file.close();                     //Closing the input file
  return vec;                       //Returning the vector
}

//Simply display function to display the int matrix.
void displayMatrix(vector<vector<int>> vec){
  cout << "Matrix: " << endl;
  for (int i = 0; i < vec.size(); ++i){         //Looping through the 2 dimensional vector.
    for (int j = 0; j < vec[i].size(); ++j){
      cout << vec[i][j] << " ";                 //Displaying the contents of the vector.
    }
    cout << endl;
  }
}

//Function to display the list of values and their position
void displayList(int list[], int count, int proc){
  for(int i = 0; i < count; i++){
    cout << "Processor " << proc << " found " << list[i] << " at (" << list[i+1] << "," << list[i+2] << ")" << endl;
    i+=2;
  }
}

int main(){
  int rank, size;         //Initializing variables
  int count1, count2, count3;
  int n, m;

  MPI_Init(NULL, NULL);   //Starting the MPI envirnment
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);   //Obtaining the rank and size
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(rank == 0 && size != 4){     //Checking for the correct number of processors
    cout << "Error: Must use 4 processors" << endl;
    return 0;
  }

  if(rank == 0){    //Obtaining the number of rows and columns
    cout << "Enter the number of rows n: " << endl;
    cin >> n;
    cout << "Enter the number of columns m: " << endl;
    cin >> m;
  }

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);   //Broadcasting the rows
  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);   //Broadcasting the columns

  vector<vector<int>> vec = readInput(n,m);   //Reading in the matrix
  int list1[n*m];         //Initializing list arrays
  int list2[n*m];
  int list3[n*m];

  if(rank == 1){    //Checking that the processor rank is 1
    count1 = 0;
    for(int i = 0; i < vec.size(); i++){    //Looping through the matrix rows
      if(i%size == 1 || i%size == 0){   //Using a mod operation to split work
        for(int j = 0; j < vec[i].size(); j++){   //Looping through the columns
          if(vec[i][j] != 0){         //Checking is a value is not 0
            list1[count1] = vec[i][j];    //Adding a value and a position to a list
            list1[count1+1] = i;
            list1[count1+2] = j;
            count1 += 3;
          }
        }
      }
    }
  }

  MPI_Bcast(&count1, 1, MPI_INT, 1, MPI_COMM_WORLD);    //Bradcasting the array and its size
  MPI_Bcast(&list1, count1, MPI_INT, 1, MPI_COMM_WORLD);

  if(rank == 2){    //Checking that the processor rank is 2
    count2 = 0;
    for(int i = 0; i < vec.size(); i++){    //Looping through the matrix rows
      if(i%size == 2){      //Using a mod operation to split work
        for(int j = 0; j < vec[i].size(); j++){ //Looping through the columns
          if(vec[i][j] != 0){     //Checking is a value is not 0
            list2[count2] = vec[i][j];    //Adding a value and a position to a list
            list2[count2+1] = i;
            list2[count2+2] = j;
            count2 += 3;
          }
        }
      }
    }
  }

  MPI_Bcast(&count2, 1, MPI_INT, 2, MPI_COMM_WORLD);  //Bradcasting the array and its size
  MPI_Bcast(&list2, count2, MPI_INT, 2, MPI_COMM_WORLD);

  if(rank == 3){    //Checking that the processor rank is 3
    count3 = 0;
    for(int i = 0; i < vec.size(); i++){    //Looping through the matrix rows
      if(i%size == 3){        //Using a mod operation to split work
        for(int j = 0; j < vec[i].size(); j++){ //Looping through the columns
          if(vec[i][j] != 0){       //Checking is a value is not 0
            list3[count3] = vec[i][j];   //Adding a value and a position to a list
            list3[count3+1] = i;
            list3[count3+2] = j;
            count3 += 3;
          }
        }
      }
    }
  }

  MPI_Bcast(&count3, 1, MPI_INT, 3, MPI_COMM_WORLD);      //Bradcasting the array and its size
  MPI_Bcast(&list3, count3, MPI_INT, 3, MPI_COMM_WORLD);

  if(rank == 0){    //Checking that the processor rank is 0
    displayMatrix(vec);   //Displaying the matrix
    displayList(list1, count1, 1);   //Displaying nonzero values and their position
    displayList(list2, count2, 2);
    displayList(list3, count3, 3);
  }

  MPI_Finalize();     //Exiting the MPI environment
  return 0;     //Exiting program
}
